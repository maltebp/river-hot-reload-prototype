#include <river/game_objects/game_object.hpp>

using namespace rv;

GameObject::GameObject(const GameObjectArgs& args) 
    :   context(args.game_object_context),
        id(args.game_object_id)
{ }