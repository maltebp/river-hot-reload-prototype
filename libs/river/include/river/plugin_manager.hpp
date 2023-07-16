#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <functional>

#include <river/core.hpp>
#include <river/plugin_system.hpp>
#include <river/plugin_system_ref.hpp>


namespace rv {

    class Plugin;
    class MainPlugin;
    class PluginSystem;

    class PluginManager {
    public:

        RV_API void register_plugin(const std::string& name, const std::vector<std::string>& dependencies);

        RV_API void load_plugin(const std::string& name);

        template<typename P>
        P* get_plugin() {
            return (P*)get_plugin(typeid(P).name());
        }

        template<class S, typename... Args>
        PluginSystemRef<S> create_system() {
            return this->create_system(
                typeid(S).name(), 
                [&](PluginSystemType* type, PluginSystem::Id id) { 
                    return new S(type, id, this, Args...);
                }
            );
        }

        RV_API [[nodiscard]] PluginSystem* get_system(const std::string& type_name, PluginSystem::Id id) const;

        RV_API void reload_changed_plugins();

        rv::MainPlugin* main_plugin = nullptr;

        // TODO: this is just temp
        bool reload_next_frame = false;

    private:

        class PluginInfo;
        class PluginSystemInfo;

    private:

        RV_API Plugin* register_and_load_plugin(const std::string& name);

        RV_API Plugin* get_plugin(const std::string& plugin_class_name);        

        PluginInfo* get_plugin_info(const std::string& name);

        Plugin* load_plugin(PluginInfo* plugin_info);

        std::unordered_set<PluginInfo*> unload_plugin(PluginInfo* plugin_info);

        RV_API [[nodiscard]] PluginSystem* create_system(const std::string& type_name, std::function<PluginSystem*(PluginSystemType*, PluginSystem::Id)> system_constructor);

    private:

        std::unordered_map<std::string, PluginInfo*> plugin_infos;
        
        std::unordered_map<std::string, PluginSystemType*> plugin_system_types;

        std::unordered_map<PluginSystem::Id, PluginSystemInfo*> plugin_system_infos;

        PluginSystem::Id next_system_id = 1;

    };

}