#pragma once

#include <string>

namespace rv {

    class PluginManager;


    class Plugin {

        friend PluginManager;

    public:

        PluginManager* get_manager() const {
            return plugin_manager;
        }   

    protected:

        Plugin() = default;

    private:

        // TODO: This should be passed as argument
        PluginManager* plugin_manager; 

    };

}