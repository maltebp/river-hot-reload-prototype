#pragma once

#include <cassert>

#include <river/core.hpp>
#include <river/game_objects/fwd.hpp>

namespace rv {

    class ComponentRefTemplateBase {
    public:

        RV_API [[nodiscard]] ComponentId get_id() const {
            return component_id;
        }

    protected:

        RV_API ComponentRefTemplateBase() { }

        RV_API ComponentRefTemplateBase(const Component* component);
        
        RV_API [[nodiscard]] Component& get_component() const;

    private:

        GameObjectContext* context = nullptr;

        ComponentId component_id = 0;

    };


    template<typename TComponent>
    class ComponentRef : ComponentRefTemplateBase {
    public:

        ComponentRef() : ComponentRefTemplateBase() { }

        ComponentRef(const TComponent* component) : ComponentRefTemplateBase(component) { }

        TComponent& get_component() const {
            TComponent& comp = (TComponent&)ComponentRefTemplateBase::get_component();
            return comp;
        };

    };

}