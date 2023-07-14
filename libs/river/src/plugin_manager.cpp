#include <river/plugin_manager.hpp>

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

    PluginInfo(const std::string& dll_name) 
        :   dll_name(dll_name)
    { }

    const std::string dll_name;

    DllHandle dll_handle;

    Plugin* plugin = nullptr;

    bool loaded = false;
    
};


void PluginManager::reload_changed_plugins() {
    for( auto [dll_name, plugin_info] : this->plugin_infos ) {
        
        bool plugin_is_changed = std::filesystem::exists(dll_name);
        if( plugin_is_changed ) {
            this->unload_plugin(plugin_info);
            this->load_plugin(plugin_info);
        }
        else {
            std::cout << dll_name << " has not changed" << std::endl;
        }
    }
}


Plugin* PluginManager::register_and_load_plugin(const std::string& dll_name) {
    assert(this->get_plugin_info(dll_name) == nullptr);
            
    PluginInfo* plugin_info = new PluginInfo(dll_name);
    this->plugin_infos[dll_name] = plugin_info;

    return this->load_plugin(plugin_info);
}


Plugin* PluginManager::get_plugin(const std::string& dll_name) {
    PluginInfo* plugin_info = this->get_plugin_info(dll_name);
    assert(plugin_info != nullptr);
    assert(plugin_info->loaded);
    return plugin_info->plugin;
}


PluginManager::PluginInfo* PluginManager::get_plugin_info(const std::string& dll_name) {
    auto it = this->plugin_infos.find(dll_name);
    if( it == this->plugin_infos.end() ) return nullptr;
    return it->second;
}


Plugin* PluginManager::load_plugin(PluginManager::PluginInfo* plugin_info) {
    using plugin_start_function = Plugin* (*)();

    if( plugin_info->loaded ) return plugin_info->plugin;

    std::filesystem::path hotload_directory = "hotloaded";
    if( !std::filesystem::exists(hotload_directory) ) {
        std::filesystem::create_directory(hotload_directory);
    }
    assert(std::filesystem::is_directory(hotload_directory));

    std::string pdb_name = plugin_info->dll_name.substr(0, plugin_info->dll_name.length() - 3) + "pdb";
    
    std::filesystem::path moved_dll_path = hotload_directory / plugin_info->dll_name;
    std::filesystem::path moved_pdb_path = hotload_directory / pdb_name;
    
    if( std::filesystem::exists(plugin_info->dll_name) ) {
        
        if( std::filesystem::exists(moved_dll_path) ) {
            std::filesystem::remove(moved_dll_path);
        }

        if( std::filesystem::exists(moved_pdb_path) ) {
            std::filesystem::remove(moved_pdb_path);
        }
        std::filesystem::rename(plugin_info->dll_name, moved_dll_path);

        if( std::filesystem::exists(moved_pdb_path) ) {
            std::filesystem::rename(pdb_name, moved_pdb_path);
        }
    }

    HINSTANCE dll_handle = LoadLibraryA(moved_dll_path.string().c_str());
    if( dll_handle == nullptr ) {
        DWORD error_code = GetLastError();
        std::cout << "Error loading dll '" << plugin_info->dll_name << "' - error code: " << error_code << std::endl;
        assert(false);
    }

    plugin_start_function start_plugin_func = 
        (plugin_start_function)GetProcAddress(dll_handle, "plugin_start");           

    if( start_plugin_func == nullptr ) {
        DWORD error_code = GetLastError();
        std::cout << "Error loading plugin_start: " << error_code << std::endl;
        assert(false);
    }

    Plugin* plugin = (Plugin*)start_plugin_func();     
    assert(plugin != nullptr);

    plugin_info->dll_handle = dll_handle;
    plugin_info->loaded = true;
    plugin_info->plugin = plugin;

    plugin->plugin_manager = this;

    if( plugin->is_entry_point ) {
        assert(entry_point == nullptr);
        entry_point = (EntryPointPlugin*)plugin;
    }

    std::cout << "  Loaded '" << plugin_info->dll_name << "'" << std::endl;

    return plugin_info->plugin;
}



 void PluginManager::unload_plugin(PluginManager::PluginInfo* plugin_info) {
    assert(plugin_info->loaded);

    bool dll_unloaded = FreeLibrary(plugin_info->dll_handle);
    if( !dll_unloaded ) {
        DWORD error_code = GetLastError();
        std::cout << "Error unloading plugin: " << error_code << std::endl;
        assert(false);
    }

    plugin_info->loaded = false;
    plugin_info->plugin = nullptr;
    plugin_info->dll_handle = { };

    std::cout << "  Plugin unloaded" << std::endl;
}


int32_t PluginManager::generate_system_id() {
    int32_t id = this->next_system_id;
    this->next_system_id++;
    return id;
}