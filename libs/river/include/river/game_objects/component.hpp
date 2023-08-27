#pragma once

#include <river/game_objects/fwd.hpp>

namespace rv {

    class ComponentArgs {
    public:

        ComponentId id;

        GameObjectId game_object_id;

        GameObjectContext& game_object_context;

    };

    class Component {
    public:

        const ComponentId id;

        // All these members are meant to be contained within the component group
        const GameObjectId game_object_id;
        GameObjectContext& context;

    protected:

        Component(const ComponentArgs& args) 
            :   id(args.id),
                game_object_id(args.game_object_id),
                context(args.game_object_context)
        { }

    };

}