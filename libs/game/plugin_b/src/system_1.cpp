#include <game/plugin_b/system_1.hpp>

using namespace game::plugin_b;

System1::System1(const rv::PluginSystemParameters& base_parameters)
    :   PluginSystem(base_parameters)
{ }

System1::System1(
    const rv::PluginSystemParameters& base_parameters,
    const rv::SerializedObject* serialized_object
)
    :   PluginSystem(base_parameters),
        hello_count(((rv::SerializedInt*)serialized_object->properties.at("hello_count"))->value)
{ }

void System1::hello() {
    this->hello_count++;
    std::cout << "Hello from Plugin B System 1 (count = " << this->hello_count << ")" << std::endl;    
}

void System1::serialize(rv::SerializedObject* object_to_serialize_to) const {
    object_to_serialize_to->properties["hello_count"] = new rv::SerializedInt(this->hello_count);
}