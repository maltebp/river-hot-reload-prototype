project "game.application"

    kind "ConsoleApp"
    language "C++"

    files { 
        "**.h",
        "**.cpp",
        "**.hpp"
    }

    includedirs {
        "include",
        "../../river/include",
        "../engine/include"
    }

    links "river"
    links "game.engine"

    filter "configurations:debug"
        defines { "DEBUG" }
        symbols "On"
        debugdir "%{prj.location}/bin/x64/debug/"

    filter "configurations:release"
        defines { "NDEBUG" }
        optimize "On"

    filter "platforms:x64"
        architecture "x64"