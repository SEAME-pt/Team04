#include "platform/joystick/joystick.hpp"

auto main() -> int {
    try {
        carMove car;
        joystick controller(car);

        // car.sequence(); // Uncomment to test the car sequence

        controller.listen();

    } catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    return 0;
}
