project "river"

    kind "SharedLib"
    language "C++"

    files { 
        "**.h",
        "**.cpp",
        "**.hpp"
    }

    includedirs {
        "include"
    }

    defines {
        "RV_API_IMPLEMENTATION"
    }
    
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

    filter "platforms:x64"
        architecture "x64"