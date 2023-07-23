#include <river/plugin_manager.hpp>

#include <unordered_set>
#include <iostream>
#include <filesystem>
#include <ctime>
#include <cassert>

#include <windows.h>

#include <river/plugin.hpp>
#include <river/serialization.hpp>

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

    std::vector<PluginManager::PluginSystemInfo*> systems;

    DllHandle dll_handle;

    Plugin* plugin = nullptr;

    bool loaded = false;
    
};


class PluginManager::PluginSystemInfo {
public:

    PluginSystemInfo(
        PluginSystemTypeId system_type_id, 
        PluginInfo* plugin_info, 
        PluginSystemId system_id,
        PluginSystem* system
    )
        :   system_type_id(system_type_id),
            plugin_info(plugin_info),
            system_id(system_id),
            system(system)
    { } 

    const PluginSystemTypeId system_type_id;

    PluginInfo* const plugin_info;

    const PluginSystemId system_id;

    PluginSystem* system;

    SerializedObject* serialized_system;

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

    std::unordered_set<PluginInfo*> unloaded_plugins;    
    std::unordered_set<PluginSystemInfo*> unloaded_systems;
    
    for( auto [name, plugin_info] : this->plugin_infos ) {
    
        // TODO: Uncomment this after testing
        //bool plugin_is_changed = std::filesystem::exists(name + ".dll");
        bool plugin_is_changed = true;
        if( plugin_is_changed ) {
            std::cout << "  Plugin " << name << ": has changed" << std::endl;
            this->unload_plugin(plugin_info, unloaded_plugins, unloaded_systems);
        }
        else {
            std::cout << "  Plugin " << name << ": has not changed" << std::endl;
        }
    }

    for( PluginInfo* unloaded_plugin : unloaded_plugins ) {
        this->load_plugin(unloaded_plugin);
    }

    for( PluginSystemInfo* system_info : unloaded_systems ) {
        PluginSystemType* system_type = this->plugin_system_types[system_info->system_type_id];

        system_info->system = system_type->deserialization_constructor(
            PluginSystemParameters{
                system_info->system_type_id,
                system_info->system_id, 
                this
            }, 
            system_info->serialized_system
        );
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
        assert(plugin_system_type_ids.find(plugin_system_type->class_name) == plugin_system_type_ids.end());
        plugin_system_type_ids[plugin_system_type->class_name] = plugin_system_type->type_id;

        assert(plugin_system_types.find(plugin_system_type->type_id) == plugin_system_types.end());
        plugin_system_types[plugin_system_type->type_id] = plugin_system_type;
    }

    if( plugin->is_main_plugin ) {
        assert(main_plugin == nullptr);
        main_plugin = (MainPlugin*)plugin;
    }

    std::cout << "  Plugin " << plugin_info->name << ": loaded" << std::endl;

    return plugin_info->plugin;
}

void PluginManager::unload_plugin(
            PluginManager::PluginInfo* plugin_info, 
            std::unordered_set<PluginManager::PluginInfo*>& unloaded_plugins, 
            std::unordered_set<PluginManager::PluginSystemInfo*>& unloaded_systems
) {

    if( !plugin_info->loaded ) return;

    std::cout << "  Plugin " << plugin_info->name << ": unloading" << std::endl;

    for( PluginInfo* dependant : plugin_info->dependants ) {
        this->unload_plugin(dependant, unloaded_plugins, unloaded_systems);
    }

    for( PluginSystemInfo* system_info : plugin_info->systems ) {
        unloaded_systems.insert(system_info);
        system_info->serialized_system = system_info->system->serialize();
        // TODO: Will deleting base class also delete the inheriting class?
        delete system_info->system;
    }

    for( PluginSystemType* exposed_system : plugin_info->plugin->exposed_system_types ) {
        this->plugin_system_type_ids.erase(exposed_system->class_name);
        this->plugin_system_types.erase(exposed_system->type_id);
    }

    unloaded_plugins.insert(plugin_info);

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
}


PluginSystem* PluginManager::create_system(
    const std::string& class_name,
    std::function<PluginSystem*(const PluginSystemParameters&)> system_constructor
) {   
    auto it = this->plugin_system_type_ids.find(class_name);
    assert(it != this->plugin_system_type_ids.end());

    PluginSystemTypeId type_id = it->second;

    PluginSystemId id = this->next_system_id;
    this->next_system_id++;
    assert(!this->plugin_system_infos.contains(id));

    PluginSystem* system = system_constructor(
        PluginSystemParameters{type_id, id, this}
    );

    PluginInfo* plugin_info = this->plugin_infos[type_id.get_plugin_name()];

    PluginSystemInfo* system_info = new PluginSystemInfo(
        type_id,
        plugin_info,
        id,
        system                
    );

    plugin_info->systems.push_back(system_info);

    this->plugin_system_infos[id] = system_info; 

    return system;
}


PluginSystem* PluginManager::get_system(const PluginSystemTypeId& type_id, PluginSystemId id) const {
    
    auto it = this->plugin_system_infos.find(id);
    assert(it != this->plugin_system_infos.end());

    PluginSystemInfo* plugin_system_info = it->second;
    assert(plugin_system_info->system_type_id == type_id);

    return plugin_system_info->system;            
}