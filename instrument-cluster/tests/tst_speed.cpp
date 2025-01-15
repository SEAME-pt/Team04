#include <gtest/gtest.h>

#include <iostream>
#include "../speed.h"

class TestSpeed : public ::testing::Test {
protected:
    Speed speed;

    TestSpeed() : speed(0.0) {}
};

TEST_F(TestSpeed, SetAndGetSpeed) {
    std::cout << "Setting speed..." << std::endl;
    speed.setSpeed(50.5);

    std::cout << "Getting speed..." << std::endl;
    double const currentSpeed = speed.getSpeed();
    std::cout << "Speed: " << currentSpeed << std::endl;

    EXPECT_EQ(currentSpeed, 50.5);
}

TEST_F(TestSpeed, GetInitialSpeed) {
    std::cout << "Getting initial speed..." << std::endl;
    double const initialSpeed = speed.getSpeed();
    std::cout << "Initial Speed: " << initialSpeed << std::endl;

    EXPECT_EQ(initialSpeed, 0.0);
}