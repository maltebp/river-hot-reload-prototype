#include <cassert>

#include <river/plugin_manager.hpp>
#include <river/plugin.hpp>


int main(int argc, char** argv) {
 
    rv::PluginManager plugin_manager = { };

    // These will eventually be passed via macros
    plugin_manager.register_plugin("game.plugin_b", { });
    plugin_manager.register_plugin("game.engine", { "game.plugin_a", "game.plugin_b" });
    plugin_manager.register_plugin("game.plugin_a", { });

    plugin_manager.run_update_loop();

    return 0;
}