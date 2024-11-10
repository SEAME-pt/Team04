#include "hello.hpp"
#include <iostream>
#include <string>

auto main(int argc, char** argv) -> int {
  std::cout << greet() << '\n';
  return 0;
}
