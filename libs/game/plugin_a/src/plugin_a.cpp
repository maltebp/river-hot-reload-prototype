#include <iostream>

#include <river/hello.hpp>
#include <game/plugin_a/i_plugin_a.hpp>

using namespace game::plugin_a;


class PluginA : public IPluginA {
public:

    virtual void hello() override {
        rv::hello("PluginA3");        
        rv::hello("PluginA");        
    }
    
};

PluginA* plugin = new PluginA();


extern "C" __declspec(dllexport) IPluginA* plugin_start() {
    std::cout << "  Plugin A: starting" << std::endl;
    return plugin;
}


extern "C" __declspec(dllexport) void plugin_stop() {
    std::cout << "  Plugin A: stopping" << std::endl;
    delete plugin;
}