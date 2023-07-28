#pragma once

#include <iostream>
#include <cassert>

#include <river/plugin_system_type.hpp>
#include <river/serialization.hpp>

namespace rv {

    class PluginSystemType;
    class PluginManager;

    using PluginSystemId = int32_t;

    struct PluginSystemParameters {
        PluginSystemTypeId type_id;
        PluginSystemId id;
        PluginManager* manager;
    };
    
    class PluginSystem {
    public:

        PluginSystem(const PluginSystemParameters& parameters)
            :   type_id(parameters.type_id),
                id(parameters.id),
                manager(parameters.manager)
        { 
            assert(id != 0);
            assert(!type_id.is_empty());
            assert(manager != nullptr);
        }

        SerializedObject* serialize() const {
            // TODO: Consider removing this serialized data (its never really used, and not really needed)
            SerializedObject* serialized_object = new SerializedObject();
            serialized_object->properties["type_id"] = this->type_id.serialize();
            serialized_object->properties["id"] = new SerializedInt(id);
            serialize(serialized_object);
            return serialized_object;
        }

    public:

        const PluginSystemTypeId type_id;

        const PluginSystemId id;

        PluginManager* const manager;

    protected:

        virtual void serialize(SerializedObject* object_to_serialize_to) const = 0;
 
    };

    class EntryPointSystem : public PluginSystem {
    public: 

        using PluginSystem::PluginSystem;

        enum class UpdateReturnCode {
            Continue,
            Reload,
            Shutdown
        };

    public:

        virtual UpdateReturnCode update() = 0;

    };

}