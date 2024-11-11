#include "hello.hpp"
#include <iostream>

auto main(int  /*argc*/, char**  /*argv*/) -> int {
  std::cout << greet() << '\n';
  return 0;
}
