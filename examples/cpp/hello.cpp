#include "hello.hpp"

#include <string>

std::string greet( ){
    return "Hello World!";
}

auto farewell() -> std::string { return "Bye World!"; }
