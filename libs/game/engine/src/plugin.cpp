#include <game/engine/plugin.hpp>

#include <river/plugin_manager.hpp>

using namespace game;
using namespace game::engine;

Plugin* plugin;

extern "C" __declspec(dllexport) rv::Plugin* plugin_start(
    rv::PluginManager* manager, 
    rv::Plugin** dependencies, 
    int dependencies_count
) {
    plugin = new Plugin(
        manager,
        typeid(Plugin).name(),
        "game.engine",
        std::vector<rv::Plugin*>(dependencies, dependencies + dependencies_count)
    );

    plugin->register_system<EntryPoint>("EntryPoint");

    // TODO: Ensure that passed dependencies match expected

    // TODO: Test if this works, or if it has to be cast to rv::MainPlugin
    return (rv::Plugin*)plugin;
}

extern "C" __declspec(dllexport) void plugin_stop() {
    delete plugin;
}