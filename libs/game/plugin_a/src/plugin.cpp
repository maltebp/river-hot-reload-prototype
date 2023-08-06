#include <iostream>

#include <river/plugin.hpp>
#include <river/plugin_system.hpp>
#include <river/hello.hpp>

#include <game/plugin_a/system_1.hpp>

using namespace game::plugin_a;


rv::Plugin* plugin;

extern "C" __declspec(dllexport) rv::Plugin* plugin_start(
    rv::PluginManager* manager, 
    rv::Plugin** dependencies, 
    int dependencies_count
) {
    plugin = new rv::Plugin(
        manager,
        "game.plugin_a",
        std::vector<rv::Plugin*>(dependencies, dependencies + dependencies_count)
    );

    plugin->register_system<System1>("System1");

    // TODO: Ensure that passed dependencies match expected

    return plugin;
}

extern "C" __declspec(dllexport) void plugin_stop() {
    delete plugin;
}