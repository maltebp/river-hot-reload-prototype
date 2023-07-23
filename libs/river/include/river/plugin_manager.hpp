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
        PluginSystemRef<S> create_system(Args... args) {
            return this->create_system(
                typeid(S).name(), 

                // Passing this "construction lambda" to allow for
                // forward declaring create_system method
                [&](const PluginSystemParameters& parameters) { 
                    return new S(parameters, args...);
                }
            );
        }

        RV_API [[nodiscard]] PluginSystem* get_system(
            const PluginSystemTypeId& type_id, PluginSystemId id) const;

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

        void unload_plugin(
            PluginInfo* plugin_info, 
            std::unordered_set<PluginInfo*>& plugins, 
            std::unordered_set<PluginSystemInfo*>& unloaded_systems
        );

        RV_API [[nodiscard]] PluginSystem* create_system(
            const std::string& type_name,
            std::function<PluginSystem*(const PluginSystemParameters&)> system_constructor
        );

    private:

         // TODO: This map should be mapped with "non-class" name (like PluginSystemTypeId)
        std::unordered_map<std::string, PluginInfo*> plugin_infos;
        
        // Mapping from system class name
        std::unordered_map<std::string, PluginSystemTypeId> plugin_system_type_ids;

        std::unordered_map<PluginSystemTypeId, PluginSystemType*> plugin_system_types;

        std::unordered_map<PluginSystemId, PluginSystemInfo*> plugin_system_infos;

        PluginSystemId next_system_id = 1;

    };

}