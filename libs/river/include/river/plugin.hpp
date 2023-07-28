#pragma once

#include <string>
#include <vector>

#include <river/plugin_manager.hpp>
#include <river/plugin_system_type.hpp>
#include <river/plugin_system_ref.hpp>

namespace rv {

    class Plugin;

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

        const bool is_entry_point = true;

        std::vector<PluginSystemType*> exposed_system_types;

    protected:

        Plugin(
            PluginManager* manager, 
            std::string class_name,
            std::string name, 
            std::vector<Plugin*> dependencies,
            bool is_entry_point
        ) 
            :   manager(manager),
                class_name(class_name),
                name(name),
                dependencies(dependencies),
                is_entry_point(is_entry_point)
        { }

    };

    class EntryPointPluginBase : public Plugin {
    public:

        EntryPointPluginBase(
            PluginManager* manager, 
            std::string class_name,
            std::string name, 
            std::vector<Plugin*> dependencies
        )
            :   Plugin(manager, class_name, name, dependencies, true)
        { }

        virtual PluginSystemRef<EntryPointSystem> create_entry_point_system() = 0;
        
    };

    template<typename S>
    class EntryPointPlugin : public EntryPointPluginBase {
    public:

        using EntryPointPluginBase::EntryPointPluginBase;

    public:

        virtual PluginSystemRef<EntryPointSystem> create_entry_point_system() override {
            return this->manager->create_system<S>();
        }

    };

}