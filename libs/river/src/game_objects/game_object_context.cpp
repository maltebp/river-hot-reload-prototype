#include <river/game_objects/game_object_context.hpp>

#include <cassert>

#include <river/plugin_manager.hpp>
#include <river/game_objects/game_object.hpp>
#include <river/game_objects/component.hpp>

using namespace rv;

struct GameObjectContext::GameObjectInfo {
    std::string type_name;
    GameObject* game_object;
    const SerializedList* serialized_args;
    SerializedObject* serialized_object = nullptr;
};

GameObjectContext::GameObjectContext(PluginManager& plugin_manager)  
    :   plugin_manager(plugin_manager) 
{
    plugin_manager.register_game_object_context(this);
}

GameObject* GameObjectContext::create_game_object(const std::string& typeid_name, const SerializedList* serialized_args) {
    this->next_game_object_id++;
    GameObjectId id = this->next_game_object_id;

    std::string type_name = plugin_manager.get_game_object_type_name(typeid_name);
    GameObjectTypeInfo type_info = plugin_manager.get_game_object_type_info(type_name);

    GameObjectArgs base_args{*this, id};
    GameObject* game_object = type_info.constructor_proxy(&base_args, serialized_args);
    GameObjectInfo* game_object_info = new GameObjectInfo();
    game_object_info->type_name = type_name;
    game_object_info->serialized_args = serialized_args;
    game_object_info->game_object = game_object;

    game_objects[id] = game_object_info;
    return game_object;
}

void GameObjectContext::unload_objects()
{
    assert(!objects_are_unloaded);
    objects_are_unloaded = true;
    for( auto& [object_id, object_info] : this->game_objects ) {
        object_info->serialized_object = object_info->game_object->serialize();
        delete object_info->game_object;
        object_info->game_object = nullptr;
    }
}

void GameObjectContext::load_objects()
{
    assert(objects_are_unloaded);
    objects_are_unloaded = false;
    for( auto& [object_id, object_info] : this->game_objects ) {
        GameObjectTypeInfo type_info = plugin_manager.get_game_object_type_info(object_info->type_name);
        GameObjectArgs base_args{*this, object_id};
        object_info->game_object = type_info.constructor_proxy(&base_args, object_info->serialized_args);
        object_info->game_object->deserialize(object_info->serialized_object);
    }
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

GameObject* GameObjectContext::get_game_object_internal(GameObjectId id) const {
    assert(game_objects.contains(id));
    return this->game_objects.at(id)->game_object;
}

