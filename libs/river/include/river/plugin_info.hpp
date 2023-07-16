#pragma once

#include <string>
#include <vector>

#include <windows.h>
typedef HINSTANCE DllHandle;

namespace rv {

    class Plugin;

    class PluginInfo {
    public:

        PluginInfo(const std::string& name, const std::vector<std::string>& dependencies) 
            :   name(name),
                dependencies(dependencies)
        { }

        const std::string name;

        const std::vector<std::string> dependencies;

        std::vector<PluginInfo*> dependants;

        DllHandle dll_handle;

        Plugin* plugin = nullptr;

        bool loaded = false;
        
    };

}