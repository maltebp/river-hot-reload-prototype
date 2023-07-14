project "game.engine"

    kind "SharedLib"
    language "C++"

    files { 
        "**.h",
        "**.cpp",
        "**.hpp"
    }

    includedirs {
        "include",
        "../plugin_a/include",
        "../plugin_b/include",
        "../../river/include"
    }

    links "river"
    links "game.plugin_a"
    links "game.plugin_b"

    defines {
        "GAME_ENGINE_API_IMPLEMENTATION "
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter "platforms:x64"
        architecture "x64"