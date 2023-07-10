workspace "river_hot_reload_prototype"
    location "build/river_hot_reload_prototype"
    platforms { "x64" }
    cppdialect "C++20"
    configurations { "debug", "release" }

include("libs/river")
include("libs/game/application")
include("libs/game/engine")
include("libs/game/plugin_a")
include("libs/game/plugin_b")