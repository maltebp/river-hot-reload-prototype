#include <game/plugin_a/class_a.hpp>

#include <iostream>


namespace game::plugin_a {

    void ClassA::print_i() const {
        std::cout << i << std::endl;
    }

}