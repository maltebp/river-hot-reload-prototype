#pragma once

#include <string>

namespace rv {

    struct GameObjectTypeInfo {
        std::string name;
        std::string typeid_name;
        void* constructor;
    };

}