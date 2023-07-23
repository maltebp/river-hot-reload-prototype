#include <game/plugin_a/system_1.hpp>

#include <iostream>

using namespace game::plugin_a;

System1::System1(
    const rv::PluginSystemParameters& base_parameters,
    int some_id
) 
    :   PluginSystem(base_parameters),
        some_id(some_id)
{ }

System1::System1(
    const rv::PluginSystemParameters& base_parameters,
    const rv::SerializedObject* serialized_object
)
    :   PluginSystem(base_parameters),
        some_id(((rv::SerializedInt*)serialized_object->properties.at("some_id"))->value),
        hello_count(((rv::SerializedInt*)serialized_object->properties.at("hello_count"))->value)
{ }

void System1::hello() {
    this->hello_count++;
    std::cout << "Hello from system 1.1 (id = " << some_id << ", count = " << this->hello_count << ")" << std::endl;    
}

int System1::get_hello_count() const {
    return hello_count;
}

void System1::serialize(rv::SerializedObject* object_to_serialize_to) const {
    object_to_serialize_to->properties["some_id"] = new rv::SerializedInt(this->some_id);
    object_to_serialize_to->properties["hello_count"] = new rv::SerializedInt(this->hello_count);
}