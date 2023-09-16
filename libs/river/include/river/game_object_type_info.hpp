#pragma once

#include <string>

namespace rv {

    class GameObject;
    struct GameObjectArgs;
    class SerializedList;

    using GameObjectConstructorProxy =
        GameObject* (*)(const GameObjectArgs* base_args, const SerializedList* serialized_args);

    struct GameObjectTypeInfo {
        std::string name;
        std::string typeid_name;
        GameObjectConstructorProxy constructor_proxy;
    };

}