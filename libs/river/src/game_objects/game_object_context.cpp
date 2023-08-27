#include <river/game_objects/game_object_context.hpp>

#include <cassert>

#include <river/game_objects/component.hpp>

using namespace rv;

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