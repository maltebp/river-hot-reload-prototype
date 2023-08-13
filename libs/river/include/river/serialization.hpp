#pragma once

#include <iostream>
#include <unordered_map>

#include <river/core.hpp>

namespace rv {

    class SerializedElement {
    public:

        RV_API virtual ~SerializedElement() = 0;

    public:

        std::string type_name;
        
    };

    class SerializedObject : public SerializedElement {
    public:

        std::unordered_map<std::string, SerializedElement*> properties;

    };

    class SerializedInt : public SerializedElement {
    public:

        SerializedInt(int value) : value(value) { }
        
        const int value;

    };

    class SerializedString : public SerializedElement {
    public:

        SerializedString(std::string value) : value(value) { }
        
        const std::string value;

    };

    class SerializedBool : public SerializedElement {
    public:

        SerializedBool(bool value) : value(value) { }
        
        const bool value;

    };

}