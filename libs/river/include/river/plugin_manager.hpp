#pragma once

#include <string>
#include <unordered_map>

#include <river/core.hpp>


namespace rv {

    class PluginManager {
    public:

        template<typename P>
        P* register_and_load_plugin() {
            return (P*)register_and_load_plugin(P::dll_name);
        }

        template<typename P>
        P* get_plugin() {            
            return (P*)get_plugin(P::dll_name);
        }

        RV_API void reload_changed_plugins();

    private:

        class PluginInfo;

    private:

        RV_API void* register_and_load_plugin(const std::string& dll_name);

        RV_API void* get_plugin(const std::string& dll_name);        

        PluginInfo* get_plugin_info(const std::string& dll_name);

        void* load_plugin(PluginInfo* plugin_info);

        void unload_plugin(PluginInfo* plugin_info);

    private:

        std::unordered_map<std::string, PluginInfo*> plugin_infos;

    };

}