#include <river/plugin_manager.hpp>

#include <unordered_set>
#include <iostream>
#include <filesystem>
#include <ctime>
#include <cassert>

#include <windows.h>

#include <river/plugin.hpp>

using namespace rv;


typedef HINSTANCE DllHandle;

class PluginManager::PluginInfo {
public:

    PluginInfo(const std::string& name, const std::vector<std::string>& dependencies) 
        :   name(name),
            dependencies(dependencies)
    { }

    const std::string name;

    const std::vector<std::string> dependencies;

    std::vector<PluginInfo*> dependants;

    DllHandle dll_handle;

    Plugin* plugin = nullptr;

    bool loaded = false;
    
};


class PluginManager::PluginSystemInfo {
public:

    PluginSystemInfo(
        PluginSystemType* system_type, 
        PluginInfo* plugin_info, 
        PluginSystem::Id system_id,
        PluginSystem* system
    )
        :   system_type(system_type),
            plugin_info(plugin_info),
            system_id(system_id),
            system(system)
    { } 

    PluginSystemType* const system_type;

    PluginInfo* const plugin_info;

    const PluginSystem::Id system_id;

    PluginSystem* system;

};

void PluginManager::register_plugin(const std::string& name, const std::vector<std::string>& dependencies) {
    assert(this->plugin_infos.find(name) == this->plugin_infos.end());
    PluginInfo* plugin_info = new PluginInfo(name, dependencies);

    // Setup dependants
    for( const auto& [existing_plugin_name, existing_plugin_info] : this->plugin_infos ) {
        bool existing_is_dependency = std::find(dependencies.begin(), dependencies.end(), (existing_plugin_name)) != dependencies.end();        
        if( existing_is_dependency ) {
            existing_plugin_info->dependants.push_back(plugin_info);
            continue;    
        }

        bool existing_is_dependant = std::find(
            existing_plugin_info->dependencies.begin(),
            existing_plugin_info->dependencies.end(),
            name
        ) != existing_plugin_info->dependencies.end();
        if( existing_is_dependant ) {
            plugin_info->dependants.push_back(existing_plugin_info);
        }
    }

    this->plugin_infos[name] = plugin_info;
}

void PluginManager::load_plugin(const std::string& name) {
    PluginInfo* plugin_info = this->get_plugin_info(name);
    assert(plugin_info != nullptr);
    this->load_plugin(plugin_info);
}

void PluginManager::reload_changed_plugins() {

    std::unordered_set<PluginInfo*> all_unloaded_plugins;
    
    for( auto [name, plugin_info] : this->plugin_infos ) {
    
        bool plugin_is_changed = std::filesystem::exists(name + ".dll");
        if( plugin_is_changed ) {
            std::cout << "  Plugin " << name << ": has changed" << std::endl;
            std::unordered_set<PluginInfo*> unloaded_plugins = this->unload_plugin(plugin_info);
            all_unloaded_plugins.insert(unloaded_plugins.begin(), unloaded_plugins.end());
        }
        else {
            std::cout << "  Plugin " << name << ": has not changed" << std::endl;
        }
    }

    for( PluginInfo* unloaded_plugin : all_unloaded_plugins ) {
        this->load_plugin(unloaded_plugin);
    }
}

Plugin* PluginManager::get_plugin(const std::string& plugin_class_name) {
    for( const auto [name, plugin_info]  : this->plugin_infos ) {
        if( plugin_info->plugin == nullptr ) continue;
        if( plugin_info->plugin->class_name == plugin_class_name) continue;
        return plugin_info->plugin; 
    }
    assert(false);
    return nullptr;
}


PluginManager::PluginInfo* PluginManager::get_plugin_info(const std::string& dll_name) {
    auto it = this->plugin_infos.find(dll_name);
    if( it == this->plugin_infos.end() ) return nullptr;
    return it->second;
}


Plugin* PluginManager::load_plugin(PluginManager::PluginInfo* plugin_info) {
    using plugin_start_function = Plugin* (*)(PluginManager*, Plugin**, int);

    assert(plugin_info != nullptr);

    if( plugin_info->loaded ) return plugin_info->plugin;

    std::cout << "  Plugin " << plugin_info->name << ": loading" << std::endl;

    // Load dependencies
    std::vector<Plugin*> dependencies;
    for( const std::string& dependency_name : plugin_info->dependencies ) {
        PluginInfo* dependency_info = this->get_plugin_info(dependency_name);
        assert(dependency_info != nullptr);
        Plugin* dependency = load_plugin(dependency_info);
        dependencies.push_back(dependency);
    }

    std::filesystem::path hotload_directory = "hotloaded";
    if( !std::filesystem::exists(hotload_directory) ) {
        std::filesystem::create_directory(hotload_directory);
    }
    assert(std::filesystem::is_directory(hotload_directory));

    std::string dll_name = plugin_info->name + ".dll";
    std::string pdb_name = plugin_info->name + ".pdb";
    
    std::filesystem::path moved_dll_path = hotload_directory / dll_name;
    std::filesystem::path moved_pdb_path = hotload_directory / pdb_name;
    
    if( std::filesystem::exists(dll_name) ) {
        
        if( std::filesystem::exists(moved_dll_path) ) {
            std::filesystem::remove(moved_dll_path);
        }

        if( std::filesystem::exists(moved_pdb_path) ) {
            std::filesystem::remove(moved_pdb_path);
        }
        std::filesystem::rename(dll_name, moved_dll_path);

        if( std::filesystem::exists(pdb_name) ) {
            std::filesystem::rename(pdb_name, moved_pdb_path);
        }
    }

    HINSTANCE dll_handle = LoadLibraryA(moved_dll_path.string().c_str());
    if( dll_handle == nullptr ) {
        DWORD error_code = GetLastError();
        std::cout << "  Plugin " << plugin_info->name << ": error loading .dll (error code: " << error_code << ")" << std::endl;
        assert(false);
    }

    plugin_start_function start_plugin_func = 
        (plugin_start_function)GetProcAddress(dll_handle, "plugin_start");           

    if( start_plugin_func == nullptr ) {
        DWORD error_code = GetLastError();
        std::cout << "  Plugin " << plugin_info->name << ": error loading plugin_start function (error code: " << error_code << ")" << std::endl;
        assert(false);
    }

    Plugin* plugin = start_plugin_func(this, dependencies.data(), (int)dependencies.size());     
    assert(plugin != nullptr);

    plugin_info->dll_handle = dll_handle;
    plugin_info->loaded = true;
    plugin_info->plugin = plugin;
    
    for( PluginSystemType* const plugin_system_type : plugin->exposed_system_types ) {
        assert(plugin_system_types.find(plugin_system_type->class_name) == plugin_system_types.end());
        plugin_system_types[plugin_system_type->class_name] = plugin_system_type;
    }

    if( plugin->is_main_plugin ) {
        assert(main_plugin == nullptr);
        main_plugin = (MainPlugin*)plugin;
    }

    std::cout << "  Plugin " << plugin_info->name << ": loaded" << std::endl;

    return plugin_info->plugin;
}

std::unordered_set<PluginManager::PluginInfo*> PluginManager::unload_plugin(
    PluginManager::PluginInfo* plugin_info
) {
    if( !plugin_info->loaded ) return { };

    std::cout << "  Plugin " << plugin_info->name << ": unloading" << std::endl;

    std::unordered_set<PluginInfo*> unloaded_plugins { plugin_info };

    // Load dependencies
    for( PluginInfo* dependant : plugin_info->dependants ) {
        std::unordered_set<PluginInfo*> dependants_unloaded_plugins = this->unload_plugin(dependant);
        unloaded_plugins.insert(dependants_unloaded_plugins.begin(), dependants_unloaded_plugins.end());
    }

    bool dll_unloaded = FreeLibrary(plugin_info->dll_handle);
    if( !dll_unloaded ) {
        DWORD error_code = GetLastError();
        std::cout << "  Plugin " << plugin_info->name << ": error unloading (error code: " << error_code << ")" << std::endl;
        assert(false);
    }

    if( this->main_plugin == plugin_info->plugin ) {
        this->main_plugin = nullptr;
    }

    plugin_info->loaded = false;
    plugin_info->plugin = nullptr;
    plugin_info->dll_handle = { };

    std::cout << "  Plugin " << plugin_info->name << ": unloaded" << std::endl;

    return unloaded_plugins;
}


PluginSystem* PluginManager::create_system(const std::string& type_name, std::function<PluginSystem*(PluginSystemType*, PluginSystem::Id)> system_constructor) {
    
    auto it = this->plugin_system_types.find(type_name);
    assert(it != this->plugin_system_types.end());

    PluginSystemType* type = it->second;
    
    PluginSystem::Id id = this->next_system_id;
    PluginSystem* system = system_constructor(type, id);
    
    // TODO: type->plugin is nullptr
    PluginSystemInfo* system_info = new PluginSystemInfo(
        type,
        this->plugin_infos[type->plugin->name],
        id,
        system                
    );

    this->plugin_system_infos[id] = system_info; 

    return system;
}


PluginSystem* PluginManager::get_system(const std::string& type_name, PluginSystem::Id id) const {
    auto it = this->plugin_system_infos.find(id);
    assert(it != this->plugin_system_infos.end());

    PluginSystemInfo* plugin_system_info = it->second;
    assert(plugin_system_info->system_type->class_name == type_name);

    return plugin_system_info->system;            
}