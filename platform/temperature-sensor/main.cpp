#include <csignal>
#include <cstdint>
#include <iostream>
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
            publisher.send({3, temperature++});
        } catch (zmq::error_t &e) {
            if (ETERM == e.num()) {
                break;
            }
        }

        if (static_cast<bool>(interrupted)) {
            std::cout << "interrupt received, killing program...\n";
            break;
        }
        sleep(1);
    }

    return 0;
}
