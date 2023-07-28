#pragma once

#include <string>

#include <river/plugin.hpp>

#include <game/engine/entry_point.hpp>

namespace game::engine {

    class Plugin : public rv::EntryPointPlugin<EntryPoint> {
        using rv::EntryPointPlugin<EntryPoint>::EntryPointPlugin;
    };

}
