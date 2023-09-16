#pragma once

#include <river/game_objects/game_object.hpp>
#include <river/game_objects/component.hpp>

namespace game::plugin_a {

    class ComponentA1 : public rv::Component {
    public:

        ComponentA1(const rv::ComponentArgs& args, int value) 
            :   rv::Component(args),
                value(value)
        { }

        const int value;

    };

    class ComponentA2 : public rv::Component {
    public:

        ComponentA2(const rv::ComponentArgs& args, int value, ComponentA1& a1) 
            :   rv::Component(args),
                value(value),
                a1(a1)
        { }

        const int value;

        ComponentA1& a1;

    };

    class GameObjectA1 : public rv::GameObject {
    public:

        GameObjectA1(const rv::GameObjectArgs& base_args, int number) 
            :   GameObject(base_args),
                a1(add_component<ComponentA1>(0, number)),
                a2(add_component<ComponentA2>(0, number*2, a1))
        { }

        rv::SerializedObject* serialize() const override {
            rv::SerializedObject* serialized_object = new rv::SerializedObject();
            serialized_object->properties["modifiable_value"] = new rv::SerializedInt(modifiable_value);
            return serialized_object;
        }

        void deserialize(const rv::SerializedObject* serialized_object) override {
            modifiable_value = ((const rv::SerializedInt*)(serialized_object->properties.at("modifiable_value")))->value;
        }

        ComponentA1& a1;

        ComponentA2& a2;

        int modifiable_value = 0;

    };

}