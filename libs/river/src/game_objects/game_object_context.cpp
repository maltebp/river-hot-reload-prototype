#include <river/game_objects/game_object_context.hpp>

#include <cassert>

#include <river/plugin_manager.hpp>
#include <river/game_objects/component.hpp>

using namespace rv;

struct GameObjectContext::GameObjectInfo {
    std::string name;
    GameObject* game_object;
    GameObjectContext::CreateFunction create_function;
};

GameObject* GameObjectContext::create_game_object(const std::string& typeid_name, CreateFunction create_function) {
    this->next_game_object_id++;
    GameObjectId id = this->next_game_object_id;

    std::string type_name = plugin_manager.get_game_object_type_name(typeid_name);
    GameObjectTypeInfo type_info = plugin_manager.get_game_object_type_info(type_name);

    GameObjectArgs base_args{*this, id};
    GameObject* game_object = create_function(type_info.constructor, &base_args);
    GameObjectInfo* game_object_info = new GameObjectInfo();
    game_object_info->name = type_name;
    game_object_info->create_function = create_function;
    game_object_info->game_object = game_object;

    game_objects[id] = game_object_info;
    return game_object;
}

Component& GameObjectContext::get_component(ComponentId id) {
    assert(components.contains(id));
    Component& comp = *(components[id]);
    return comp;
}

ComponentId GameObjectContext::generate_component_id() {
    next_component_id++;
    return next_component_id;
}

void GameObjectContext::register_component(Component* component) {
    assert(!this->components.contains(component->id));
    this->components[component->id] = component;
}

