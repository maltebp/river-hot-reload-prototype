#pragma once

#include <string>


namespace game::plugin_a {

    class IPluginA {
    public:

        virtual void hello() = 0;

    protected:

        IPluginA() = default;

    public:

        inline static const std::string dll_name = "game.plugin_a.dll";

    };

}