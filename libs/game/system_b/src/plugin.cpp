#include <iostream>

#include <river/hello.hpp>

#include <game/system_a/class_a.hpp>


extern "C" __declspec(dllexport) void start_plugin() {
    rv::hello("System B");

    game::system_a::ClassA c = { };

    c.i++;

    c.print_i();
}