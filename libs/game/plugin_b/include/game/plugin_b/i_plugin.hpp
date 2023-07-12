#pragma once

#include <string>

#include <river/plugin.hpp>


namespace game::plugin_b {

    class IPlugin : rv::Plugin {
    public:

        virtual void hello() = 0;

    public:

        inline static const std::string dll_name = "game.plugin_b.dll";

    };

}