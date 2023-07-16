#pragma once

#include <river/plugin_system.hpp>

#include <game/plugin_a/api.hpp>


namespace game::plugin_a {

    class System1 : public rv::PluginSystem {
    public:

        using rv::PluginSystem::PluginSystem;

        GAME_PLUGIN_A_API void hello();
        
    };

}