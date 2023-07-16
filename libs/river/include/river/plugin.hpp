#pragma once

#include <string>
#include <vector>
#include <functional>

namespace rv {

    class Plugin;
    class PluginManager;
    
    class PluginSystemType {
    public:

        PluginSystemType(
            Plugin* plugin,
            std::string class_name,
            std::function<void*()> deserialization_constructor
        ) 
            :   plugin(plugin),
                class_name(class_name),
                deserialization_constructor(deserialization_constructor)
        { }

    public:

        Plugin* const plugin;

        const std::string class_name;

        const std::function<void*()> deserialization_constructor;

    };


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

        // TODO: Try to encapsulate this a bit (e.g. friend function or something)
        template <class S1, class S2, class... Sn>
        void register_systems() {
            this->register_systems<S1>();
            this->register_systems<S2, Sn...>();
        }

        template <class S>
        void register_systems() {
            exposed_system_types.push_back(
                new PluginSystemType(
                    this,
                    typeid(S).name(), 
                    [](){ return new S(); }
                )
            );
        }

    public:
            
        PluginManager* const manager; 

        const std::string class_name;

        const std::string name;

        const std::vector<Plugin*> dependencies; 

        const bool is_main_plugin = true;

        std::vector<PluginSystemType*> exposed_system_types;


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