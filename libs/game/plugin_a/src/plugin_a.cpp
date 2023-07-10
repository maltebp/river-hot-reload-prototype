#include <river/hello.hpp>
#include <game/plugin_a/i_plugin_a.hpp>

using namespace game::plugin_a;


class PluginA : public IPluginA {
public:

    virtual void hello() override {
        rv::hello("PluginA");        
    }

};


extern "C" __declspec(dllexport) IPluginA* plugin_start() {
    PluginA* plugin = new PluginA();
    return plugin;
}