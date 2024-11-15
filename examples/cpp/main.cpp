#include <iostream>

#include "hello.hpp"

auto main(int /*argc*/, char** /*argv*/) -> int {
    std::cout << greet() << '\n';
    return 0;
}
