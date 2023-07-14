#pragma once

#include <string>

#include <river/plugin.hpp>

#include <game/plugin_b/api.hpp>


namespace game::plugin_b {

    class Plugin : public rv::Plugin {
    public:

        GAME_PLUGIN_B_API void hello();

    public:

        inline static const std::string dll_name = "game.plugin_b.dll";

    };

}