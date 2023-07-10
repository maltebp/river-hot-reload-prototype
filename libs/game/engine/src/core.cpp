#include <game/engine/core.hpp>

#include <iostream>
#include <cassert>

#include <windows.h>

#include <river/plugin_manager.hpp>
#include <river/plugin.hpp>
#include <game/system_a/i_plugin_a.hpp>


namespace game::engine::core {

    void run() {
        rv::PluginManager plugin_manager = { };

        game::system_a::IPluginA* plugin_a = plugin_manager.register_and_load_plugin<game::system_a::IPluginA>();
        plugin_a->hello();
        
    }

}