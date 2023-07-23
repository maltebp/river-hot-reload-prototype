#pragma once

#include <string>
#include <vector>

#include <river/plugin_system_type.hpp>

namespace rv {

    class Plugin;
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

        template <class S>
        void register_system(const std::string& system_name) {
            PluginSystemType::DeserializationConstructor deserialization_constructor = [](
                const PluginSystemParameters& base_parameters,
                const SerializedObject* serialized_system
            ) {
                return new S(base_parameters, serialized_system);
            };

            this->exposed_system_types.push_back(
                new PluginSystemType(
                    PluginSystemTypeId(this->name, system_name),
                    typeid(S).name(), 
                    this,
                    deserialization_constructor    
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