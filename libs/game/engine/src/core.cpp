#include <game/engine/core.hpp>

#include <iostream>
#include <cassert>

#include <windows.h>

#include <river/plugin.hpp>


namespace game::engine::core {

    void run() {
        HINSTANCE dll = LoadLibraryA("game.system_a.dll");
        assert(dll != 0);

        rv::start_plugin start_plugin_func = (rv::start_plugin)GetProcAddress(dll, "start_plugin");

        if( start_plugin_func == nullptr ) {
            DWORD dw = GetLastError();
            std::cout << "Error loading function: " << dw << std::endl;
            return;
        }

        assert(start_plugin_func != 0);
        start_plugin_func();     
    }

}