#include <river/plugin_system_ref.hpp>

#include <river/plugin_manager.hpp>
#include <river/serialization.hpp>

using namespace rv;


PluginSystemRefBase::PluginSystemRefBase(PluginSystem* system) 
    :   type_id(system->type_id),
        manager(system->manager),
        id(system->id)
{ 
    assert(system != nullptr);
}

PluginSystemRefBase::PluginSystemRefBase(
    PluginManager* manager, 
    SerializedObject* serialized_system_ref
)
    :   manager(manager),
        type_id((SerializedObject*)serialized_system_ref->properties["type_id"]),
        id(((SerializedInt*)serialized_system_ref->properties["id"])->value)
{ }

PluginSystem* PluginSystemRefBase::get_system_raw() const {
    assert(!this->is_empty());
    return this->manager->get_system(this->type_id, this->id);    
}


SerializedObject* PluginSystemRefBase::serialize() const {
    SerializedObject* serialized_object = new SerializedObject();
    serialized_object->properties["type_id"] = this->type_id.serialize();
    serialized_object->properties["id"] = new SerializedInt(this->id);
    return serialized_object;
}