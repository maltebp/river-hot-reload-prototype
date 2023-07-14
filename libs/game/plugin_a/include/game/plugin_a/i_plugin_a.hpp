#pragma once

#include <string>

#include <river/plugin.hpp>

#include <game/plugin_a/api.hpp>

namespace game::plugin_a {


    class PluginA : public rv::Plugin {
    public:

        GAME_PLUGIN_A_API void hello();

    public:

        inline static const std::string dll_name = "game.plugin_a.dll";

    };

}