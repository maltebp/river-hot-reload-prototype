#include <iostream>

#include <river/hello.hpp>
#include <game/plugin_a/plugin.hpp>

using namespace game::plugin_a;


void Plugin::hello() {
    rv::hello("Plugin3");        
    rv::hello("Plugin");        
}


Plugin* plugin = new Plugin();

extern "C" __declspec(dllexport) rv::Plugin* plugin_start() {
    std::cout << "  Plugin A: starting" << std::endl;
    return plugin;
}


extern "C" __declspec(dllexport) void plugin_stop() {
    std::cout << "  Plugin A: stopping" << std::endl;
    delete plugin;
}