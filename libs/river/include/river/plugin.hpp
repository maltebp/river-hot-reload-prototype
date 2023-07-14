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

    public:

        const bool is_entry_point;

        const std::string hello = "Hello World";

    protected:

        Plugin() : is_entry_point(false) { }

        Plugin(bool is_entry_point)
            :   is_entry_point(is_entry_point)
        { }

    private:

        // TODO: This should be passed as argument
        PluginManager* plugin_manager; 

    };


    class EntryPointPlugin : public Plugin {
    public:

        virtual bool update() = 0;

    protected:

        EntryPointPlugin()
            :   Plugin(true)
        { }

    };

}