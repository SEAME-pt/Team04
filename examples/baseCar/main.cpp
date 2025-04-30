#include "platform/joystick/joystick.hpp"
#include "platform/remoteMove/remoteMove.hpp"

auto main() -> int {
    try {
        carMove car;
        joystick controller(car);
        RemoteMove remote(car, "tcp://*:5555");
        remote.start();
        // car.sequence(); // Uncomment to test the car sequence

        controller.listen();

    } catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    return 0;
}
