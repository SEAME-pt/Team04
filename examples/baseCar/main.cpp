#include "platform/joystick/joystick.hpp"

auto main() -> int {
    try {
        carMove car;
        joystick controller(car);

        controller.listen();

    } catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    return 0;
}
