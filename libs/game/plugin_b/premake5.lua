project "game.plugin_b"

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
        "../plugin_a/include"
    }

    links "river"

    defines {
        "GAME_PLUGIN_B_API_IMPLMENTATION"
    }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter "platforms:x64"
        architecture "x64"