#pragma once

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

            assert(this->get_plugin_info<P>() == nullptr);
            
            PluginInfo* plugin_info = new PluginInfo(P::dll_name);
            this->plugin_infos[P::dll_name] = plugin_info;

            return this->load_plugin<P>();
        }

        template<typename P>
        P* reload_plugin() {
            PluginInfo* plugin_info = get_plugin_info<P>(); 
            assert(plugin_info != nullptr);
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

            return this->load_plugin<P>();
        }        

    private:

        class PluginInfo;

    private:

        template<typename P>
        PluginInfo* get_plugin_info() {
            auto it = this->plugin_infos.find(P::dll_name);
            if( it == this->plugin_infos.end() ) return nullptr;
            return it->second;
        }

        template<typename P>
        P* load_plugin() {
            using plugin_start_function = P* (*)();

            PluginInfo* plugin_info = this->get_plugin_info<P>();
            assert(plugin_info != nullptr);

            if( plugin_info->loaded ) {
                return (P*)plugin_info->plugin;
            }

            // std::time_t current_time = std::time(0);
            // std::stringstream current_time_stream;
            // std::put_time(&current_time, "%y%j%h%h%M%S");
            
            std::filesystem::path hotload_directory = "hotloaded";

            std::string pdb_name = P::dll_name.substr(0, P::dll_name.length() - 3) + "pdb";
            
            std::filesystem::path moved_dll_path = hotload_directory / P::dll_name;
            std::filesystem::path moved_pdb_path = hotload_directory / pdb_name;

            if( std::filesystem::exists(moved_dll_path) ) {
                std::filesystem::remove(moved_dll_path);
            }

            if( std::filesystem::exists(moved_pdb_path) ) {
                std::filesystem::remove(moved_pdb_path);
            }

            std::filesystem::rename(P::dll_name, moved_dll_path);
            std::filesystem::rename(pdb_name, moved_pdb_path);

            // bool file_copied = std::filesystem::copy_file(
            //     P::dll_name, copied_dll_name, std::filesystem::copy_options::overwrite_existing);

            HINSTANCE dll_handle = LoadLibraryA(moved_dll_path.string().c_str());
            if( dll_handle == nullptr ) {
                DWORD error_code = GetLastError();
                std::cout << "Error loading dll '" << P::dll_name << "' - error code: " << error_code << std::endl;
                assert(false);
            }

            plugin_start_function start_plugin_func = 
                (plugin_start_function)GetProcAddress(dll_handle, "plugin_start");           

            if( start_plugin_func == nullptr ) {
                DWORD error_code = GetLastError();
                std::cout << "Error loading plugin_start: " << error_code << std::endl;
                assert(false);
            }

            P* plugin = start_plugin_func();     
            assert(plugin != nullptr);

            plugin_info->dll_handle = dll_handle;
            plugin_info->loaded = true;
            plugin_info->plugin = plugin;

            std::cout << "  Loaded '" << P::dll_name << "'" << std::endl;

            return plugin;
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