#pragma once

#include <string>
#include <unordered_map>

#include <windows.h>


namespace rv {

    class PluginManager {
    public:

        template<typename T>
        T* register_and_load_plugin() {
            using plugin_start_function = T* (*)();

            HINSTANCE dll = LoadLibraryA(T::dll_name.c_str());
            if( dll == nullptr ) {
                DWORD error_code = GetLastError();
                std::cout << "Error loading dll '" << T::dll_name << "' - error code: " << error_code << std::endl;
                assert(false);
            }

            plugin_start_function start_plugin_func = 
                (plugin_start_function)GetProcAddress(dll, "plugin_start");           

            if( start_plugin_func == nullptr ) {
                DWORD error_code = GetLastError();
                std::cout << "Error loading plugin_start: " << error_code << std::endl;
                assert(false);
            }

            T* plugin = start_plugin_func();     
            return plugin; 
        }

    // private:

    //     // TODO: Test if this is required
    //     class PluginInfo;

    // private:

    //     PluginInfo* get_plugin_info(const std::string& dll_name);

    //     void* load_plugin(PluginInfo* plugin_info);

    // private:

    //     std::unordered_map<std::string, const PluginInfo*> plugins;

    // private:

    //     typedef HINSTANCE DllHandle;

    //     class PluginInfo {
    //     public:

    //         std::string dll_name;
            
    //     };

    };

}