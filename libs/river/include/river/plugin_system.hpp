#pragma once

#include <vector>


namespace rv {

    class Plugin;
    
    
    class PluginSystem {
    protected:
        
        PluginSystem(Plugin* plugin);

    private:

        const int32_t id;

        Plugin* const plugin;

        std::vector<PluginSystem*> dependencies;

    };

}