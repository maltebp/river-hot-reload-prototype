#pragma once

#include <string>
#include <unordered_map>

#include <river/core.hpp>


namespace rv {

    class Plugin;
    class EntryPointPlugin;


    class PluginManager {
    public:

        void register_plugin(const std::string& dll_name) {
            this->register_and_load_plugin(dll_name);
        }

        template<typename P>
        P* get_plugin() {            
            return (P*)get_plugin(P::dll_name);
        }

        RV_API void reload_changed_plugins();

        int32_t generate_system_id();

        rv::EntryPointPlugin* entry_point = nullptr;

        // TODO: this is just temp
        bool reload_next_frame = false;

    private:

        class PluginInfo;

    private:

        RV_API Plugin* register_and_load_plugin(const std::string& dll_name);

        RV_API Plugin* get_plugin(const std::string& dll_name);        

        PluginInfo* get_plugin_info(const std::string& dll_name);

        Plugin* load_plugin(PluginInfo* plugin_info);

        void unload_plugin(PluginInfo* plugin_info);

    private:

        std::unordered_map<std::string, PluginInfo*> plugin_infos;

        int32_t next_system_id = 1;

    };

}