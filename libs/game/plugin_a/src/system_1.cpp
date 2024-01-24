#include <game/plugin_a/system_1.hpp>

#include <iostream>

#include <river/plugin_system.hpp>
#include <river/plugin_manager.hpp>

#include <game/plugin_b/system_1.hpp>
#include <game/plugin_b/system_2.hpp>

#include <game/plugin_a/game_objects.hpp>

using namespace game::plugin_a;

System1::System1(
    const rv::PluginSystemParameters& base_parameters,
    int some_id
) 
    :   PluginSystem(base_parameters),
        some_id(some_id),
        b_system_1(base_parameters.manager->create_system<plugin_b::System1>()),
        b_system_2(base_parameters.manager->create_system<plugin_b::System2>()),
        game_object_context(*this->manager)
{ 
    test_object_id = game_object_context.create_game_object<GameObjectA1>(42)->id;
}

System1::System1(
    const rv::PluginSystemParameters& base_parameters,
    const rv::SerializedObject* serialized_object
)
    :   PluginSystem(base_parameters),
        some_id(((rv::SerializedInt*)serialized_object->properties.at("some_id"))->value),
        hello_count(((rv::SerializedInt*)serialized_object->properties.at("hello_count"))->value),
        b_system_1(base_parameters.manager, (rv::SerializedObject*)serialized_object->properties.at("b_system_1")),
        b_system_2(base_parameters.manager, (rv::SerializedObject*)serialized_object->properties.at("b_system_2")),
        game_object_context(*this->manager)
{ 
    test_object_id = game_object_context.create_game_object<GameObjectA1>(42)->id;
}

void System1::hello() {

    std::cout << "Game objects " << std::endl;
    {
        GameObjectA1* game_object = game_object_context.get_game_object<GameObjectA1>(test_object_id);
        std::cout << "GameObjectA1.value = " << game_object->a1.value << std::endl;
        std::cout << "GameObjectA2.value = " << game_object->a2.value << std::endl;
        std::cout << "GameObjectA2.a1.value = " << game_object->a2.a1.value << std::endl;
        game_object->modifiable_value++;
        std::cout << "GameObjectA1.modifiable_value = " << game_object->modifiable_value << std::endl;
        std::cout << "GameObjectA1.new_value = " << game_object->new_value << std::endl;
    }

    this->hello_count++;
    std::cout << "Hello from PluginA's system 1 (id = " << some_id << ", count = " << this->hello_count << ")" << std::endl;    
    this->b_system_1.get_system()->hello(); 
    this->b_system_2.get_system()->hello(); 
}

int System1::get_hello_count() const {
    return hello_count;
}

void System1::serialize(rv::SerializedObject* object_to_serialize_to) const {
    object_to_serialize_to->properties["some_id"] = new rv::SerializedInt(this->some_id);
    object_to_serialize_to->properties["hello_count"] = new rv::SerializedInt(this->hello_count);
    object_to_serialize_to->properties["b_system_1"] = this->b_system_1.serialize();
    object_to_serialize_to->properties["b_system_2"] = this->b_system_2.serialize();
}