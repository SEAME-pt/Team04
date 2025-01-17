#include <iostream>

#include "platform/joystick/joystick.hpp"

carMove car;

// Function to handle left stick horizontal axis movements (steering)
void moveLeftandRight(int value) {
    float fvalue = value * 1.0;  // Raw value
    std::cout << "Before normalization: " << fvalue << std::endl;

    // Normalize to a 45-degree range
    fvalue = fvalue / 32000.0 * 45;
    std::cout << "Servo angle moved to: " << fvalue << " degrees" << std::endl;

    car.setServoAngle(fvalue);
}

// Function to handle right stick vertical axis movements (engine control)
void moveForwardandBackward(int value) {
    value -= 16319;              // Adjust for neutral position
    value = (value / 165) * -1;  // Normalize and invert for correct direction

    std::cout << "Engine speed moved to: " << value << std::endl;
    car.setMotorSpeed(value);
}

auto main() -> int {
    try {
        joystick controller(car);

        // Map left stick horizontal axis to control the servo (steering)
        controller.setAxisAction(SDL_CONTROLLER_AXIS_LEFTX, moveLeftandRight);

        // Map right stick vertical axis to control the engine (forward/reverse)
        controller.setAxisAction(SDL_CONTROLLER_AXIS_RIGHTY, moveForwardandBackward);

        // Start listening for events
        controller.listen();

    } catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
