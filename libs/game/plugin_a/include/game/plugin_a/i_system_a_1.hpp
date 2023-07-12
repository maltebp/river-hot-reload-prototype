#pragma once

#include <river/plugin_system.hpp>


namespace game::plugin_a {

    class ISystemA1 : public rv::PluginSystem {
    public:

        using rv::PluginSystem::PluginSystem;

        virtual void hello() = 0;
        
    };

}