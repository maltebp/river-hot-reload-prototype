#pragma once

#include <iostream>
#include <cassert>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <ctime>

#include <windows.h>


namespace rv {

    class PluginManager {
    public:

        template<typename P>
        P* register_and_load_plugin() {
            assert(this->get_plugin_info(P::dll_name) == nullptr);
            
            PluginInfo* plugin_info = new PluginInfo(P::dll_name);
            this->plugin_infos[P::dll_name] = plugin_info;

            return (P*)this->load_plugin(plugin_info);
        }

        template<typename P>
        P* get_plugin() {
            PluginInfo* plugin_info = this->get_plugin_info(P::dll_name);
            assert(plugin_info != nullptr);
            assert(plugin_info->loaded);
            return (P*)plugin_info->plugin;            
        }

        void reload_changed_plugins() {
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

    private:

        class PluginInfo;

    private:

        PluginInfo* get_plugin_info(const std::string& dll_name) {
            auto it = this->plugin_infos.find(dll_name);
            if( it == this->plugin_infos.end() ) return nullptr;
            return it->second;
        }

        void* load_plugin(PluginInfo* plugin_info) {
            using plugin_start_function = void* (*)();

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
            }

            if( std::filesystem::exists(moved_pdb_path) ) {
                std::filesystem::rename(pdb_name, moved_pdb_path);
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

            void* plugin = start_plugin_func();     
            assert(plugin != nullptr);

            plugin_info->dll_handle = dll_handle;
            plugin_info->loaded = true;
            plugin_info->plugin = plugin;

            std::cout << "  Loaded '" << plugin_info->dll_name << "'" << std::endl;

            return plugin_info->plugin;
        }

        void unload_plugin(PluginInfo* plugin_info) {
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

    private:

        std::unordered_map<std::string, PluginInfo*> plugin_infos;

    private:

        typedef HINSTANCE DllHandle;

        class PluginInfo {
        public:

            PluginInfo(const std::string& dll_name) 
                :   dll_name(dll_name)
            { }

            const std::string dll_name;

            DllHandle dll_handle;

            void* plugin = nullptr;

            bool loaded = false;
            
        };

    };

}