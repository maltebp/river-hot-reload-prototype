#include <game/engine/core.hpp>

#include <iostream>
#include <cassert>

#include <windows.h>

#include <river/plugin_manager.hpp>
#include <river/plugin.hpp>
#include <game/plugin_a/i_plugin_a.hpp>


namespace game::engine::core {

    void run() {
        rv::PluginManager plugin_manager = { };

        game::plugin_a::IPluginA* plugin_a = plugin_manager.register_and_load_plugin<game::plugin_a::IPluginA>();
        
        while( true ) {
            std::cout << std::endl << "> ";

            std::string input;
            std::getline(std::cin, input);

            if( input == "hello" ) {
                plugin_a->hello();
            }
            else if( input == "quit" ) {
                std::cout << "Quitting.." << std::endl;
                break;
            }
            else {
                std::cout << "Error: unknown command '" << input << "'" << std::endl;
            }
        }
        
    }

}