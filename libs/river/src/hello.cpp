#include <river/hello.hpp>

#include <iostream>


namespace rv {

	void hello(std::string sender) {
		std::cout << "Hello, from " << sender << std::endl;
	}

}