#include <iostream>

#include <river/hello.hpp>

#include <game/plugin_b/i_plugin.hpp>

using namespace game::plugin_b;


class Plugin : public IPlugin {
public:

    virtual void hello() override {
        rv::hello("Plugin B 2");        
    }
    
};

Plugin* plugin = nullptr;


extern "C" __declspec(dllexport) IPlugin* plugin_start() {
    plugin = new Plugin();
    std::cout << "  Plugin B: starting" << std::endl;
    return plugin;
}


extern "C" __declspec(dllexport) void plugin_stop() {
    std::cout << "  Plugin B: stopping" << std::endl;
    delete plugin;
}