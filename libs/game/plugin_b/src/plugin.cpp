#include <iostream>

#include <river/hello.hpp>

#include <game/plugin_b/i_plugin.hpp>

using namespace game::plugin_b;


void Plugin::hello() {
    rv::hello("Plugin B 2");
}


Plugin* plugin = nullptr;


extern "C" __declspec(dllexport) rv::Plugin* plugin_start() {
    plugin = new Plugin();
    std::cout << "  Plugin B: starting" << std::endl;
    return (rv::Plugin*)plugin;
}


extern "C" __declspec(dllexport) void plugin_stop() {
    std::cout << "  Plugin B: stopping" << std::endl;
    delete plugin;
}