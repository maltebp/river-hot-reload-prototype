#include <game/plugin_a/system_1.hpp>

#include <iostream>

#include <river/plugin_system.hpp>
#include <river/plugin_manager.hpp>

#include <game/plugin_b/system_1.hpp>

using namespace game::plugin_a;

System1::System1(
    const rv::PluginSystemParameters& base_parameters,
    int some_id
) 
    :   PluginSystem(base_parameters),
        some_id(some_id),
        b_system_1(base_parameters.manager->create_system<plugin_b::System1>())
{ }

System1::System1(
    const rv::PluginSystemParameters& base_parameters,
    const rv::SerializedObject* serialized_object
)
    :   PluginSystem(base_parameters),
        some_id(((rv::SerializedInt*)serialized_object->properties.at("some_id"))->value),
        hello_count(((rv::SerializedInt*)serialized_object->properties.at("hello_count"))->value),
        b_system_1(base_parameters.manager, (rv::SerializedObject*)serialized_object->properties.at("b_system_1"))
{ }

void System1::hello() {
    this->hello_count++;
    std::cout << "Hello from PluginA's system 1 (id = " << some_id << ", count = " << this->hello_count << ")" << std::endl;    
    this->b_system_1.get_system()->hello(); 
}

int System1::get_hello_count() const {
    return hello_count;
}

void System1::serialize(rv::SerializedObject* object_to_serialize_to) const {
    object_to_serialize_to->properties["some_id"] = new rv::SerializedInt(this->some_id);
    object_to_serialize_to->properties["hello_count"] = new rv::SerializedInt(this->hello_count);
    object_to_serialize_to->properties["b_system_1"] = this->b_system_1.serialize();
}