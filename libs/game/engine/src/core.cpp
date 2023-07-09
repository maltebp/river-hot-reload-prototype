#include <game/engine/core.hpp>

#include <iostream>
#include <cassert>

#include <windows.h>

#include <river/plugin.hpp>


void load_plugin(const char* name) {
    HINSTANCE dll = LoadLibraryA(name);
    assert(dll != 0);

    rv::start_plugin start_plugin_func = (rv::start_plugin)GetProcAddress(dll, "start_plugin");

    if( start_plugin_func == nullptr ) {
        DWORD dw = GetLastError();
        std::cout << "Error loading function: " << dw << std::endl;
        assert(false);
    }

    assert(start_plugin_func != 0);
    start_plugin_func();     
}

namespace game::engine::core {

    void run() {
        load_plugin("game.system_a.dll");
        load_plugin("game.system_b.dll");
    }

}