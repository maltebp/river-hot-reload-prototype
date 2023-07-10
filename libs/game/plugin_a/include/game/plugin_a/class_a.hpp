#pragma once

#include <game/plugin_a/api.hpp>

namespace game::plugin_a {

    class GAME_PLUGIN_A_API ClassA {
    public:

        void print_i() const;

        int i = 0;

    };

}