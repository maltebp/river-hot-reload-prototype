#include <river/game_objects/component_ref.hpp>

#include <cassert>

#include <river/game_objects/component.hpp>
#include <river/game_objects/game_object_context.hpp>

using namespace rv;

ComponentRefTemplateBase::ComponentRefTemplateBase(const Component* component) 
    :   component_id(component->id),
        context(&component->context)
{ }

Component& ComponentRefTemplateBase::get_component() const {
    assert(this->component_id != 0);
    assert(this->context != nullptr);
    return context->get_component(this->component_id);
}

