#include "hello.hpp"

#include <string>

auto greet() -> std::string { return "Hello World!"; }

std::string greet(){
    return "Hello World!";
}

auto farewell() -> std::string { return "Bye World!"; }
