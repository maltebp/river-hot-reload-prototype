#pragma once

#include <game/plugin_a/i_system_a_1.hpp>

namespace game::plugin_a {


    class SystemA1 : public ISystemA1 {
    public:

        SystemA1(rv::Plugin* plugin)
            :   ISystemA1(plugin)
        { }

        using ISystemA1::ISystemA1;
        
    };

}