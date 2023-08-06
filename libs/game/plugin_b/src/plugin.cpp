#include <iostream>

#include <river/hello.hpp>

#include <game/plugin_b/plugin.hpp>
#include <game/plugin_b/system_1.hpp>
#include <game/plugin_b/system_2.hpp>

using namespace game::plugin_b;


void Plugin::hello() {
    rv::hello("Plugin B 2");
}

Plugin* plugin;

extern "C" __declspec(dllexport) rv::Plugin* plugin_start(
    rv::PluginManager* manager, 
    rv::Plugin** dependencies, 
    int dependencies_count
) {
    plugin = new Plugin(
        manager,
        typeid(Plugin).name(),
        "game.plugin_b",
        std::vector<rv::Plugin*>(dependencies, dependencies + dependencies_count)
    );

    plugin->register_system<System1>("System1");
    plugin->register_system<System2>("System2");

    // TODO: Ensure that passed dependencies match expected

    return (rv::Plugin*)plugin;
}

extern "C" __declspec(dllexport) void plugin_stop() {
    delete plugin;
}