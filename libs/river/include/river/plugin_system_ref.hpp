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
            :   type_id(),
                manager(nullptr),
                id(0)
        { }

        PluginSystemRef(PluginSystem* system) 
            :   type_id(system->type_id),
                manager(system->manager),
                id(system->id)
        { }

        // TODO: Copy constructor

        // TODO: Assignment operator

        [[nodiscard]] S* get_system() const {
            assert(id != 0);
            return (S*)manager->get_system(type_id, id);
        }

        [[nodiscard]] PluginManager* get_manager() const {
            return manager;
        }

        [[nodiscard]] PluginSystemId get_id() const {
            return id;
        }

        // TODO: Implement -> operator

    private:
        
    private:

        PluginSystemTypeId type_id;

        PluginManager* manager;

        PluginSystemId id;

    };

}