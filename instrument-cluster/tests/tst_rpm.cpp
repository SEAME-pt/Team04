#include <gtest/gtest.h>
#include <iostream>
#include "../rpm.h"

class TestRPM : public ::testing::Test {
   protected:
    Rpm rpm;

    TestRPM() : rpm(0) {}
};

TEST_F(TestRPM, SetAndGetRPM) {
    std::cout << "Setting RPM..." << "\n";
    rpm.setRPM(3000);

    std::cout << "Getting RPM..." << "\n";
    int const currentRPM = rpm.getRPM();
    std::cout << "RPM: " << currentRPM << "\n";

    EXPECT_EQ(currentRPM, 3000);
}

TEST_F(TestRPM, GetInitialRPM) {
    std::cout << "Getting initial RPM..." << "\n";
    int const initialRPM = rpm.getRPM();
    std::cout << "Initial RPM: " << initialRPM << "\n";

    EXPECT_EQ(initialRPM, 0);
}
