#include <gtest/gtest.h>
#include <iostream>
#include "speed.h"

class TestSpeed : public ::testing::Test {
   protected:
    Speed speed;

    TestSpeed() : speed(0.0) {}
};

TEST_F(TestSpeed, SetAndGetSpeed) {
    std::cout << "Setting speed..." << "\n";
    speed.setSpeed(50.5);

    std::cout << "Getting speed..." <<  "\n";
    double currentSpeed = speed.getSpeed();
    std::cout << "Speed: " << currentSpeed <<  "\n";

    EXPECT_EQ(currentSpeed, 50.5);
}

TEST_F(TestSpeed, GetInitialSpeed) {
    std::cout << "Getting initial speed..." <<  "\n";
    double initialSpeed = speed.getSpeed();
    std::cout << "Initial Speed: " << initialSpeed <<  "\n";

    EXPECT_EQ(initialSpeed, 0.0);
}
