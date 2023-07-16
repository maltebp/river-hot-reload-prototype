#pragma once

#include <cassert>

#include <river/plugin_system.hpp>
#include <river/plugin_manager.hpp>

namespace rv {

    class PluginSystem;
    class PluginSystemType;

    // TODO: Should probably extract non-template base class to prevent excessive header inclusion
    template<class S>
    class PluginSystemRef {

        friend class PluginManager;

    public:

        PluginSystemRef() 
            :   type(nullptr),
                manager(nullptr),
                id(0)
        { }

        PluginSystemRef(PluginSystem* system) 
            :   type(system->type),
                manager(system->manager),
                id(system->id)
        { }

        // TODO: Copy constructor

        // TODO: Assignment operator

        [[nodiscard]] S* get_system() const {
            assert(id != 0);
            return (S*)manager->get_system(typeid(S).name(), id);
        }

        [[nodiscard]] PluginManager* get_manager() const {
            return manager;
        }

        [[nodiscard]] PluginSystem::Id get_id() const {
            return id;
        }

        // TODO: Implement -> operator

    private:
        
    private:

        PluginSystemType* type;

        PluginManager* manager;

        PluginSystem::Id id;

    };

}