#include <gtest/gtest.h>

#include "hello.hpp"

TEST(HelloWorldTest, GreetTest) { EXPECT_EQ(greet(), "Hello World!"); }

TEST(HelloWorldTest, FarewellTest) { EXPECT_EQ(farewell(), "Bye World!"); }
