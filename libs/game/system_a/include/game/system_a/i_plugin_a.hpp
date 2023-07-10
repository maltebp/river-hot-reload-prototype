#pragma once

#include <string>


namespace game::system_a {

    class IPluginA {
    public:

        virtual void hello() = 0;

    public:

        inline static const std::string dll_name = "game.system_a.dll";

    };

}