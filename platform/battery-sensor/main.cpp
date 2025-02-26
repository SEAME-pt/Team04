#include <cmath>
#include <csignal>
#include <cstdint>
#include <iostream>
#include <zmq.hpp>

#include "ina219/ina219.hpp"
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

auto main() -> int {
    float shunt_ohms = 0.1F;
    float max_expected_amps = 3.2F;
    float initial_charge = 15000.0F;
    float interval = 1000.0F;

    INA219 i(shunt_ohms, max_expected_amps);
    i.configure(RANGE_16V, GAIN_8_320MV, ADC_12BIT, ADC_12BIT);

    std::cout << "time_s,bus_voltage_V,supply_voltage_V,shunt_voltage_mV,current_mA,power_mW,"
                 "remaining_percentage"
              << std::endl;

    int c = 0;
    float remaining_charge = initial_charge;
    while (c < 5) {
        float current = i.current();
        remaining_charge = remaining_charge - ((current / 1000.0F) * (interval / 1000.0F));
        float percentage = (100.0F * remaining_charge) / initial_charge;
        std::cout << c << "," << i.voltage() << "," << i.supplyVoltage() << "," << i.shuntVoltage()
                  << "," << i.current() << "," << i.power() << "," << percentage << std::endl;
        c++;
        i.sleep();
        usleep(1000000);  // 1s
        i.wake();
    }

    // Connect to zmq
    zmq::context_t context(1);
    MQ::ZeroMQSocket publisher{context, zmq::socket_type::pub};
    if (!publisher.connect("ipc:///tmp/speed.ipc")) {
        return 1;
    }

    uint8_t percentage;

    catchSignals();
    while (true) {
        float current = i.current();
        remaining_charge = remaining_charge - ((current / 1000.0F) * (interval / 1000.0F));
        percentage = static_cast<uint8_t>(roundf((100.0F * remaining_charge) / initial_charge));

        try {
            publisher.send({2, percentage});
        } catch (zmq::error_t &e) {
            if (ETERM == e.num()) {
                break;
            }
        }

        if (static_cast<bool>(interrupted)) {
            std::cout << "interrupt received, killing program...\n";
            break;
        }
        i.sleep();
        sleep(10);
        i.wake();
    }

    return 0;
}
