#pragma once

#include <unordered_map>

#include <river/core.hpp>
#include <river/game_objects/fwd.hpp>
#include <river/game_objects/game_object_args.hpp>

namespace rv {

    class GameObjectContext {
        friend GameObject;
    public:

        template<typename TGameObject, typename ... TArgs>
        TGameObject* create_game_object(TArgs... args) {
            GameObjectId id = this->next_game_object_id;
            this->next_game_object_id++;
            TGameObject* game_object = new TGameObject(GameObjectArgs{*this, id}, args...);
            game_objects[id] = game_object;
            return game_object;
        }

        RV_API Component& get_component(ComponentId id);

    private:

        RV_API ComponentId generate_component_id();

        RV_API void register_component(Component* component);

    private:

        std::unordered_map<GameObjectId, GameObject*> game_objects;

        std::unordered_map<ComponentId, Component*> components;

        GameObjectId next_game_object_id = 1;

        ComponentId next_component_id = 1;

    };

}