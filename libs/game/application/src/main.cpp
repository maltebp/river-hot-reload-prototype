#include <cassert>

#include <river/plugin_manager.hpp>
#include <river/plugin.hpp>


int main(int argc, char** argv) {
 
    rv::PluginManager plugin_manager = { };

    // These will eventually be passed via macros
    plugin_manager.register_plugin("game.plugin_b", { });
    plugin_manager.register_plugin("game.engine", { "game.plugin_a", "game.plugin_b" });
    plugin_manager.register_plugin("game.plugin_a", { });

    plugin_manager.load_plugin("game.plugin_b");
    plugin_manager.load_plugin("game.engine");
    plugin_manager.load_plugin("game.plugin_a");

    assert(plugin_manager.main_plugin != nullptr);

    while( true ) {

        if( plugin_manager.reload_next_frame ) {
            plugin_manager.reload_next_frame = false;
            plugin_manager.reload_changed_plugins();
        }

        plugin_manager.main_plugin->update();
    }

    return 0;
}