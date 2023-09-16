#pragma once

#include <river/serialization.hpp>

namespace rv
{
    
    template<>
    static SerializedElement* serialize<int>(const int& value) {
        return new SerializedInt(value);
    }

    template<>
    static int deserialize<int>(const SerializedElement* serialized_element) {
        const SerializedInt* serialized_int =
            dynamic_cast<const SerializedInt*>(serialized_element);

        assert(serialized_int != nullptr && "SerializedElement is not an int");

        return serialized_int->value;
    }

    // TODO: Implement remaining types
}
