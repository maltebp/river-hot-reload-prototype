#include <game/engine/plugin.hpp>

#include <iostream>

#include <river/plugin_manager.hpp>
#include <river/hello.hpp>
#include <game/plugin_a/i_plugin_a.hpp>
#include <game/plugin_b/i_plugin.hpp>

using namespace game::engine;


const std::string Plugin::dll_name = "game.engine.dll";

bool Plugin::update() {
    
    std::cout << std::endl << "> ";

    std::string input;
    std::getline(std::cin, input);

    if( input == "hello" ) {
        this->get_manager()->get_plugin<game::plugin_a::PluginA>()->hello();
        this->get_manager()->get_plugin<game::plugin_b::Plugin>()->hello();
    }
    else if( input == "reload" ) {
        this->get_manager()->reload_next_frame = true;
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

Plugin* plugin = new Plugin();

extern "C" __declspec(dllexport) rv::Plugin* plugin_start() {
    std::cout << "  Plugin 'game.engine': starting" << std::endl;
    return plugin;
}

extern "C" __declspec(dllexport) void plugin_stop() {
    std::cout << "  Plugin 'game.engine': starting" << std::endl;
    delete plugin;
}