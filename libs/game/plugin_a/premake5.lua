project "game.plugin_a"

    kind "SharedLib"
    language "C++"

    files { 
        "**.h",
        "**.cpp",
        "**.hpp"
    }

    includedirs {
        "include",
        "../../river/include",
        "../plugin_b/include"
    }

    links "river"
    links "game.plugin_b"

    defines {
        "GAME_PLUGIN_A_API_IMPLMENTATION"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
        debugformat "c7"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter "platforms:x64"
        architecture "x64"