#pragma once

#include <river/game_objects/fwd.hpp>

#include <river/game_objects/component_ref.hpp>
#include <river/game_objects/component.hpp>
#include <river/game_objects/game_object_args.hpp>
#include <river/game_objects/game_object_context.hpp>

namespace rv {

    class GameObject {
    public:

        RV_API GameObject(const GameObjectArgs& args);

        const GameObjectId id;

        GameObjectContext& context;

    protected:

        template<typename TComponent, typename ... TArgs>
        TComponent& add_component(size_t component_group_index, TArgs&&... args) {
            // This is just prototype placeholder - this should be 
            // constructed within a component set in the game object context
            ComponentArgs component_args{context.generate_component_id(), id, context};
            TComponent* component = new TComponent(component_args, args... );
            context.register_component(component);
            return *component;  
        }

    };

}