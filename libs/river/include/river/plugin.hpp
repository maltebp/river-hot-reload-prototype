#pragma once

#include <string>
#include <vector>

namespace rv {

    class PluginManager;


    class Plugin {

        friend PluginManager;

    public:

        Plugin(
            PluginManager* manager, 
            std::string class_name, 
            std::string name, 
            std::vector<Plugin*> dependencies
        )
            :   Plugin(manager, class_name, name, dependencies, false)
        { }

    public:
            
        PluginManager* const manager; 

        const std::string class_name;

        const std::string name;

        const std::vector<Plugin*> dependencies; 

        const bool is_main_plugin = true;

    protected:

        Plugin(
            PluginManager* manager, 
            std::string class_name,
            std::string name, 
            std::vector<Plugin*> dependencies,
            bool is_main_plugin
        ) 
            :   manager(manager),
                class_name(class_name),
                name(name),
                dependencies(dependencies),
                is_main_plugin(is_main_plugin)
        { }

    };


    class MainPlugin : public Plugin {
    public:

        MainPlugin(
            PluginManager* manager, 
            std::string class_name,
            std::string name, 
            std::vector<Plugin*> dependencies
        )
            :   Plugin(manager, class_name, name, dependencies, true)
        { }

        virtual bool update() = 0;

    };

}