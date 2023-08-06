#include <iostream>

#include <river/plugin.hpp>
#include <river/hello.hpp>

#include <game/plugin_b/system_1.hpp>
#include <game/plugin_b/system_2.hpp>

using namespace game::plugin_b;


rv::Plugin* plugin = nullptr;

extern "C" __declspec(dllexport) rv::Plugin* plugin_start(
    rv::PluginManager* manager, 
    rv::Plugin** dependencies, 
    int dependencies_count
) {
    plugin = new rv::Plugin(
        manager,
        "game.plugin_b",
        std::vector<rv::Plugin*>(dependencies, dependencies + dependencies_count)
    );

    plugin->register_system<System1>("System1");
    plugin->register_system<System2>("System2");

    // TODO: Ensure that passed dependencies match expected

    return plugin;
}

extern "C" __declspec(dllexport) void plugin_stop() {
    delete plugin;
}