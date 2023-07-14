#pragma once

#include <string>

#include <river/plugin.hpp>

namespace game::engine {

    class Plugin : public rv::MainPlugin {
        using rv::MainPlugin::MainPlugin;
    public:

        virtual bool update() override;

    };

}
