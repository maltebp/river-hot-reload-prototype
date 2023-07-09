#include <iostream>

#include <river/hello.hpp>


extern "C" __declspec(dllexport) void start_plugin() {
    rv::hello("System A");
}