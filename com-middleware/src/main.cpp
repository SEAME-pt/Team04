#include <linux/can.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <zmq.hpp>

#include "can/CanDriver.hpp"
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

auto main(int argc, char **argv) -> int {
    int opt{};
    std::string interface = "can0";

    // NOLINTNEXTLINE(concurrency-mt-unsafe)
    while ((opt = getopt(argc, argv, "i:")) != -1) {  // "i:" means -i takes an argument
        switch (opt) {
            case 'i':
                interface = optarg;
                break;
            case '?':  // Unknown option
                std::cerr << "Unknown option: -" << static_cast<char>(optopt) << "\n";
                return 1;
            default:
                return 1;
        }
    }

    std::unique_ptr<candriver::CanDriver> can;
    try {
        can = std::make_unique<candriver::CanDriver>(interface, 5);
    } catch (const std::exception &exception) {
        std::cout << exception.what() << "\n";
        return 1;
    }

    struct can_frame frame {};  // Classical can frame
    int32_t num_bytes{};

    // Connect to zmq
    zmq::context_t context(1);
    MQ::ZeroMQSocket publisher{context, zmq::socket_type::pub};
    publisher.bind("ipc:///tmp/speed.ipc");

    catchSignals();
    while (true) {
        num_bytes = can->receive(&frame);

        if (num_bytes < 0) {
            if (errno == EINTR) {
                std::cout << "interrupt received, exiting gracefully..." << "\n";
                break;
            }
            if (errno != EAGAIN) {
                perror("Error reading");
            }
            continue;
        }

        // The len element contains the payload length in bytes and should be used instead of
        // can_dlc
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
        printf("0x%03X [%d] ", frame.can_id, frame.len);

        for (int i = 0; i < frame.len; i++) {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-vararg)
            std::cout << printf("%02X ", frame.data[i]);
        }

        std::cout << "\n";

        std::vector<uint8_t> message(frame.len + 1);
        message[0] = static_cast<uint8_t>(frame.can_id);
        memcpy(message.data() + 1, frame.data, static_cast<size_t>(num_bytes));

        try {
            publisher.send(message);
        } catch (zmq::error_t &e) {
            if (ETERM == e.num()) {
                break;
            }
        }

        if (static_cast<bool>(interrupted)) {
            std::cout << "interrupt received, killing program...\n";
            break;
        }
    }

    return 0;
}
