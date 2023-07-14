#pragma once

#include <string>

#include <river/plugin.hpp>

#include <game/plugin_b/api.hpp>


namespace game::plugin_b {

    class Plugin : public rv::Plugin {
        using rv::Plugin::Plugin;
    public:

        GAME_PLUGIN_B_API void hello();

    };

}