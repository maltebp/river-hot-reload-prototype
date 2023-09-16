#pragma once

#include <iostream>
#include <cassert>
#include <unordered_map>

#include <river/core.hpp>
#include <river/template_util.hpp>

namespace rv {

    enum class SerializedElementType {
        object,
        list,
        integer,
        string,
        boolean
    };

    class SerializedElement {
    public:

        RV_API virtual ~SerializedElement() = 0;

    };

    class SerializedList : public SerializedElement {
    public:

        std::vector<SerializedElement*> elements;

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

    // TODO: I'm not a complete fan of these because of the weird inheritance pattern.
    // E.g., you can (by accident) call deserialize with a SerializedList and not get
    // a compiletime error, because it will simple upcast it to a SerializedElement.
    // This will make the dynamic_cast fail, even though we already knew it would
    // be a problem.

    // Idea: Could the typed serialized elements inherit from another class 
    // SerializedValue (example name), and then have SerializedElement contain a
    // SerializedValue. This way, you cannot upcast SerializedList to SerializedElement

    template<typename T>
    static rv::SerializedElement* serialize(const T& value) { 
        static_assert(template_util::False<T>::value, "Serializer not found");
    }

    template<typename T>
    static T deserialize(SerializedObject* serialized_object) {
        static_assert(template_util::False<T>::value, "Deserializer not found");
        return {};
    }

    // TODO: This should not be specialized by outsiders - can we prevent that?
    template<typename T>
    static T deserialize(const SerializedElement* serialized_element) {
        const SerializedObject* serialized_object = 
            dynamic_cast<const SerializedObject*>(serialized_element);
        
        assert(serialized_object != nullptr && "SerializedElement is not a serialized object");
        return deserialize<T>(serialized_object);
    }    

}