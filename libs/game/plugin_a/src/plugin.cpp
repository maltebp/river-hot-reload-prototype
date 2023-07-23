#include <iostream>

#include <river/plugin_system.hpp>
#include <river/hello.hpp>

#include <game/plugin_a/plugin.hpp>
#include <game/plugin_a/system_1.hpp>

using namespace game::plugin_a;


void Plugin::hello() {
    rv::hello("Plugin3");        
    rv::hello("Plugin");        
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
        "game.plugin_a",
        std::vector<rv::Plugin*>(dependencies, dependencies + dependencies_count)
    );

    plugin->register_system<System1>("System1");

    // TODO: Ensure that passed dependencies match expected

    return (rv::Plugin*)plugin;
}

extern "C" __declspec(dllexport) void plugin_stop() {
    delete plugin;
}