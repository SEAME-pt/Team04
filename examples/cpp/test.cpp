#include <gtest/gtest.h>
#include "hello.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wglobal-constructors"
TEST(HelloWorldTest, GreetTest) {
#pragma GCC diagnostic pop
    EXPECT_EQ(greet(), "Hello World!");
}
