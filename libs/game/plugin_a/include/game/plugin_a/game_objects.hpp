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

        GameObjectA1(const rv::GameObjectArgs& args, int number) 
            :   GameObject(args),
                a1(add_component<ComponentA1>(0, number)),
                a2(add_component<ComponentA2>(0, number*2, a1))
        { }

        ComponentA1& a1;

        ComponentA2& a2;

    };

}