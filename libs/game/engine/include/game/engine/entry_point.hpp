#pragma once

#include <river/plugin_system.hpp>
#include <river/plugin_system_ref.hpp>

#include <game/plugin_a/system_1.hpp>

namespace game::engine {

    class EntryPoint : public rv::EntryPointSystem {
        
        using EntryPointSystem::EntryPointSystem;        
        
    public:

        EntryPoint(
            rv::PluginSystemParameters base_parameters,
            const rv::SerializedObject* serialized_object
        )
            :   rv::EntryPointSystem(base_parameters),
                initialized(((rv::SerializedInt*)serialized_object->properties.at("initialized"))->value),
                system_1(base_parameters.manager, (rv::SerializedObject*)serialized_object->properties.at("system_1"))
        { }

        virtual EntryPointSystem::UpdateReturnCode update() override;

    protected:
    
        virtual void serialize(rv::SerializedObject* object_to_serialize_to) const override;

    private:

        bool initialized = false;

        rv::PluginSystemRef<plugin_a::System1> system_1;

    };

}