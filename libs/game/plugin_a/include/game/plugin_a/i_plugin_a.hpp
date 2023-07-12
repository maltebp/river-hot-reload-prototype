#pragma once

#include <string>

#include <river/plugin.hpp>

namespace game::plugin_a {


    class IPluginA : public rv::Plugin {
    public:

        virtual void hello() = 0;

    public:

        inline static const std::string dll_name = "game.plugin_a.dll";

    };

}