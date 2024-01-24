#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include <functional>

#include <river/core.hpp>
#include <river/game_objects/fwd.hpp>
#include <river/game_objects/game_object_args.hpp>
#include <river/serializers.hpp>

namespace rv {

    class PluginManager;

    class GameObjectContext {
        friend GameObject;
        struct GameObjectInfo;    
    public:

        using CreateFunction = std::function<GameObject*(void*, const GameObjectArgs*)>;

        RV_API GameObjectContext(PluginManager& plugin_manager);

        template<typename TGameObject, typename ... TArgs>
        TGameObject* create_game_object(TArgs... args) {
            static_assert(std::is_constructible_v<TGameObject, const GameObjectArgs&, TArgs...>);

            std::array serialized_args{ serialize<TArgs>(args)... };
            SerializedList* serialized_args_list = new SerializedList();

            for( SerializedElement* serialized_arg : serialized_args ) {
                serialized_args_list->elements.push_back(serialized_arg);
            }

            std::string typeid_name = typeid(TGameObject).name();

            // TODO: Test if a dynamic cast works here
            TGameObject* game_object = (TGameObject*)create_game_object(typeid_name, serialized_args_list);

            return game_object;
        }

        template<typename TGameObject>
        TGameObject* get_game_object(GameObjectId id) const {
            TGameObject* casted_object = dynamic_cast<TGameObject*>(this->get_game_object_internal(id));
            assert(casted_object != nullptr);
            return casted_object;
        }

        void unload_objects();

        void load_objects();

        RV_API Component& get_component(ComponentId id);

    private:

        RV_API GameObject* create_game_object(
            const std::string& typeid_name, const SerializedList* serialized_args);

        RV_API ComponentId generate_component_id();

        RV_API void register_component(Component* component);

        RV_API GameObject* get_game_object_internal(GameObjectId id) const;

    private:

        PluginManager& plugin_manager;

        std::unordered_map<GameObjectId, GameObjectInfo*> game_objects;

        std::unordered_map<ComponentId, Component*> components;

        GameObjectId next_game_object_id = 1;

        ComponentId next_component_id = 1;

        bool objects_are_unloaded = false;

    };

}