#include <river/plugin.hpp>
#include <river/plugin_manager.hpp>

#include <game/engine/entry_point.hpp>

using namespace game;
using namespace game::engine;

rv::EntryPointPlugin<EntryPoint>* plugin;

extern "C" __declspec(dllexport) rv::Plugin* plugin_start(
    rv::PluginManager* manager, 
    rv::Plugin** dependencies, 
    int dependencies_count
) {
    plugin = new rv::EntryPointPlugin<EntryPoint>(
        manager,
        "game.engine",
        std::vector<rv::Plugin*>(dependencies, dependencies + dependencies_count)
    );

    plugin->register_system<EntryPoint>("EntryPoint");

    // TODO: Ensure that passed dependencies match expected

    return (rv::Plugin*)plugin;
}

extern "C" __declspec(dllexport) void plugin_stop() {
    delete plugin;
}