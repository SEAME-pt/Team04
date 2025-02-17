#include <csignal>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <zmq.hpp>

#include "mq/src/ZeroMQSocket.hpp"

namespace {
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
volatile std::sig_atomic_t interrupted{0};
}  // namespace

void signalHandler(int signum) {
    (void)signum;  // ignore unused variable
    interrupted = 1;
}

void catchSignals() {
    (void)std::signal(SIGINT, signalHandler);
    (void)std::signal(SIGTERM, signalHandler);
    (void)std::signal(SIGSEGV, signalHandler);
    (void)std::signal(SIGABRT, signalHandler);
}

auto getCPUTemperature() -> float {
    std::ifstream temp_file("/sys/class/thermal/thermal_zone0/temp");
    if (temp_file.is_open()) {
        std::string temp_string;
        std::getline(temp_file, temp_string);
        temp_file.close();

        try {
            // The value is in millidegrees Celsius. Divide by 1000 for degrees C.
            float temperature = std::stof(temp_string) / 1000.0F;
            return temperature;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
            return -1.0F;  // Indicate an error
        }
    } else {
        std::cerr << "Error: Could not open temperature file." << "\n";
        return -1.0F;  // Indicate an error
    }
}

auto main() -> int {
    // Connect to zmq
    zmq::context_t context(1);
    MQ::ZeroMQSocket publisher{context, zmq::socket_type::pub};
    if (!publisher.connect("ipc:///tmp/speed.ipc")) {
        return 1;
    }

    uint8_t temperature = 0;

    catchSignals();
    while (true) {
        try {
            temperature = static_cast<uint8_t>(getCPUTemperature());

            if (temperature != -1.0F) {
                publisher.send({3, temperature});
            }
        } catch (zmq::error_t& e) {
            if (ETERM == e.num()) {
                break;
            }
        }

        if (static_cast<bool>(interrupted)) {
            std::cout << "interrupt received, killing program...\n";
            break;
        }
        sleep(10);
    }

    return 0;
}
