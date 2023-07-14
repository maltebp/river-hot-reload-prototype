#include <cassert>

#include <river/plugin_manager.hpp>
#include <river/plugin.hpp>


int main(int argc, char** argv) {
 
    rv::PluginManager plugin_manager = { };

    // These will eventually be passed via macros
    plugin_manager.register_plugin("game.engine.dll");
    plugin_manager.register_plugin("game.plugin_b.dll");
    plugin_manager.register_plugin("game.plugin_a.dll");

    assert(plugin_manager.entry_point != nullptr);

    while( true ) {

        if( plugin_manager.reload_next_frame ) {
            plugin_manager.reload_next_frame = false;
            plugin_manager.reload_changed_plugins();
        }

        plugin_manager.entry_point->update();
    }

    return 0;
}