#include <game/system_a/class_a.hpp>

#include <iostream>


namespace game::system_a {

    void ClassA::print_i() const {
        std::cout << i << std::endl;
    }

}