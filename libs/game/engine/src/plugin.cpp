#include <game/engine/plugin.hpp>

#include <iostream>

#include <river/plugin_manager.hpp>
#include <river/hello.hpp>
#include <game/plugin_a/plugin.hpp>
#include <game/plugin_b/plugin.hpp>

using namespace game::engine;


bool Plugin::update() {
    
    std::cout << std::endl << "> ";

    std::string input;
    std::getline(std::cin, input);

    if( input == "hello" ) {
        this->manager->get_plugin<game::plugin_a::Plugin>()->hello();
        this->manager->get_plugin<game::plugin_b::Plugin>()->hello();
    }
    else if( input == "reload" ) {
        this->manager->reload_next_frame = true;
    }
    else if( input == "quit" ) {
        std::cout << "Quitting.." << std::endl;
        return false;
    }
    else {
        std::cout << "Error: unknown command '" << input << "'" << std::endl;
    }

    return true;
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
        "game.engine",
        std::vector<rv::Plugin*>(dependencies, dependencies + dependencies_count)
    );

    // TODO: Ensure that passed dependencies match expected

    // TODO: Test if this works, or if it has to be cast to rv::MainPlugin
    return (rv::Plugin*)plugin;
}

extern "C" __declspec(dllexport) void plugin_stop() {
    delete plugin;
}