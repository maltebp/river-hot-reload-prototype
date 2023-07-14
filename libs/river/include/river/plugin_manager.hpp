#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include <river/core.hpp>


namespace rv {

    class Plugin;
    class MainPlugin;


    class PluginManager {
    public:

        RV_API void register_plugin(const std::string& name, const std::vector<std::string>& dependencies);

        RV_API void load_plugin(const std::string& name);

        template<typename P>
        P* get_plugin() {
            return (P*)get_plugin(typeid(P).name());
        }

        RV_API void reload_changed_plugins();

        int32_t generate_system_id();

        rv::MainPlugin* main_plugin = nullptr;

        // TODO: this is just temp
        bool reload_next_frame = false;

    private:

        class PluginInfo;

    private:

        RV_API Plugin* register_and_load_plugin(const std::string& name);

        RV_API Plugin* get_plugin(const std::string& plugin_class_name);        

        PluginInfo* get_plugin_info(const std::string& name);

        Plugin* load_plugin(PluginInfo* plugin_info);

        std::unordered_set<PluginInfo*> unload_plugin(PluginInfo* plugin_info);

    private:

        std::unordered_map<std::string, PluginInfo*> plugin_infos;

        int32_t next_system_id = 1;

    };

}