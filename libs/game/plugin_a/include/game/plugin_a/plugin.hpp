#pragma once

#include <string>

#include <river/plugin.hpp>

#include <game/plugin_a/api.hpp>

namespace game::plugin_a {

    class Plugin : public rv::Plugin {
        using rv::Plugin::Plugin;
    public:

        GAME_PLUGIN_A_API void hello();

    };

}