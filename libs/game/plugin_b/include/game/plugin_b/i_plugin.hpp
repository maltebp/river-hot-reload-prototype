#pragma once

#include <string>


namespace game::plugin_b {

    class IPlugin {
    public:

        virtual void hello() = 0;

    protected:

        IPlugin() = default;

    public:

        inline static const std::string dll_name = "game.plugin_b.dll";

    };

}