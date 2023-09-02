#pragma once

#include <string>

namespace rv {

    struct GameObjectTypeInfo {
        std::string name;
        std::string real_name;
        void* constructor;
    };

}