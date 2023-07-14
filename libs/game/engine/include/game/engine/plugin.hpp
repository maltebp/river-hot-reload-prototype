#pragma once

#include <string>

#include <river/plugin.hpp>

namespace game::engine {

    class Plugin : public rv::EntryPointPlugin {
    public:

        virtual bool update() override;

        static const std::string dll_name;

    };

}
