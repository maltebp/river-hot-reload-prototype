#pragma once

#include <river/game_objects/fwd.hpp>

namespace rv {

    struct GameObjectArgs {
  
        GameObjectArgs(GameObjectContext& game_object_context, GameObjectId game_object_id) 
            :   game_object_context(game_object_context),
                game_object_id(game_object_id)
        { }
        
        GameObjectContext& game_object_context;

        GameObjectId game_object_id;
        
    };

}