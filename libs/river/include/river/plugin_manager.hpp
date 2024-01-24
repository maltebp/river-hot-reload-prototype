#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <functional>

#include <river/core.hpp>
#include <river/plugin_system.hpp>
#include <river/plugin_system_ref.hpp>
#include <river/game_object_type_info.hpp>

namespace rv {

    class Plugin;
    class PluginSystem;
    class EntryPointPluginBase;
    class EntryPointSystem;
    class GameObjectContext;

    class PluginManager {
    public:

        RV_API void run_update_loop();

        RV_API void register_plugin(const std::string& name, const std::vector<std::string>& dependencies);

        RV_API void load_plugin(const std::string& name);

        template<class S, typename... Args>
        PluginSystemRef<S> create_system(Args... args) {
            static_assert(std::is_base_of<PluginSystem, S>::value);
            S* system = (S*)this->create_system_internal(
                typeid(S).name(), 

                // Passing this "construction lambda" to allow for
                // forward declaring create_system method
                [&](const PluginSystemParameters& base_parameters) { 
                    return new S(base_parameters, args...);
                }
            );

            PluginSystemRef<S> system_ref = system;
            
            return system_ref;
        };

        RV_API [[nodiscard]] PluginSystem* get_system(
            const PluginSystemTypeId& type_id, PluginSystemId id) const;

        GameObjectTypeInfo get_game_object_type_info(const std::string& type_name) const;

        std::string get_game_object_type_name(const std::string& real_name) const;

        RV_API void reload_changed_plugins();

        void register_game_object_context(GameObjectContext* context);

    private:

        class PluginInfo;
        class PluginSystemInfo;

    private:

        Plugin* load_plugin(PluginInfo* plugin_info);

        PluginInfo* get_plugin_info(const std::string& name);

        void unload_plugin(
            PluginInfo* plugin_info, 
            std::unordered_set<PluginInfo*>& plugins, 
            std::unordered_set<PluginSystemInfo*>& unloaded_systems
        );

        RV_API [[nodiscard]] PluginSystem* create_system_internal(
            const std::string& class_name,
            std::function<PluginSystem*(const PluginSystemParameters&)> system_constructor
        );

    private:

        EntryPointPluginBase* entry_point_plugin = nullptr;

        PluginSystemRef<EntryPointSystem> entry_point_system;

         // TODO: This map should be mapped with "non-class" name (like PluginSystemTypeId)
        std::unordered_map<std::string, PluginInfo*> plugin_infos;
        
        // Mapping from system class name
        std::unordered_map<std::string, PluginSystemTypeId> plugin_system_type_ids;

        std::unordered_map<PluginSystemTypeId, PluginSystemType*> plugin_system_types;

        std::unordered_map<PluginSystemId, PluginSystemInfo*> plugin_system_infos;

        PluginSystemId next_system_id = 1;

        std::vector<GameObjectContext*> game_object_contexts;

    };

}