#pragma once

#include <iostream>
#include <cassert>

namespace rv {

    class PluginSystemType;
    class PluginManager;
    
    class PluginSystem {
    public:

        using Id = int32_t;

    public:

        PluginSystem(PluginSystemType* type, Id id, PluginManager* manager)
            :   type(type),
                id(id),
                manager(manager)
        { 
            assert(id != 0);
            assert(type != nullptr);
            assert(manager != nullptr);
        }

        PluginSystem() 
            :   type(type),
                id(id),
                manager(manager)
        { }

    public:

        const int32_t id;

        PluginManager* const manager;

        PluginSystemType* const type;
 
    };

}