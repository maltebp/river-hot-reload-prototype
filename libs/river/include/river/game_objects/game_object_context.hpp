#pragma once

#include <string>
#include <unordered_map>
#include <functional>

#include <river/core.hpp>
#include <river/game_objects/fwd.hpp>
#include <river/game_objects/game_object_args.hpp>

namespace rv {

    class PluginManager;

    class GameObjectContext {
        friend GameObject;
        struct GameObjectInfo;    
    public:

        using CreateFunction = std::function<GameObject*(void*, const GameObjectArgs*)>;

        GameObjectContext(PluginManager& plugin_manager) : plugin_manager(plugin_manager) { }

        template<typename TGameObject, typename ... TArgs>
        TGameObject* create_game_object(TArgs... args) {
            static_assert(std::is_constructible_v<TGameObject, const GameObjectArgs&, TArgs...>);
            
            CreateFunction create_function = [args...](
                void* constructor_proxy, const GameObjectArgs* base_args
            ) mutable {
                using ConstructorProxy = TGameObject* (*)(const GameObjectArgs*, TArgs...);
                ConstructorProxy typed_constructor_proxy = (ConstructorProxy)constructor_proxy;
                return (GameObject*)typed_constructor_proxy(base_args, args...);
            };

            std::string real_name = typeid(TGameObject).name();

            // TODO: Test if a dynamic cast works here
            TGameObject* game_object = (TGameObject*)create_game_object(real_name, create_function);

            return game_object;
        }

        RV_API Component& get_component(ComponentId id);

    private:

        RV_API GameObject* create_game_object(const std::string& real_name, CreateFunction create_function);

        RV_API ComponentId generate_component_id();

        RV_API void register_component(Component* component);

    private:

        PluginManager& plugin_manager;

        std::unordered_map<GameObjectId, GameObjectInfo*> game_objects;

        std::unordered_map<ComponentId, Component*> components;

        GameObjectId next_game_object_id = 1;

        ComponentId next_component_id = 1;

    };

}