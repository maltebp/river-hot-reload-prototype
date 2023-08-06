#include <game/plugin_b/system_2.hpp>

using namespace game::plugin_b;

System2::System2(const rv::PluginSystemParameters& base_parameters)
    :   PluginSystem(base_parameters)
{ }

System2::System2(
    const rv::PluginSystemParameters& base_parameters,
    const rv::SerializedObject* serialized_object
)
    :   PluginSystem(base_parameters),
        hello_count(((rv::SerializedInt*)serialized_object->properties.at("hello_count"))->value)
{ }

void System2::hello() {
    this->hello_count++;
    std::cout << "Hello from Plugin B System 2 (count = " << this->hello_count << ")" << std::endl;    
}

void System2::serialize(rv::SerializedObject* object_to_serialize_to) const {
    object_to_serialize_to->properties["hello_count"] = new rv::SerializedInt(this->hello_count);
}