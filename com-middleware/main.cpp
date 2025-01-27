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

namespace {
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
volatile std::sig_atomic_t interrupted{0};
}  // namespace

void signal_handler(int signum) {
    (void)signum;  // ignore unused variable
    interrupted = 1;
}

void catch_signals() {
    (void)std::signal(SIGINT, signal_handler);
    (void)std::signal(SIGTERM, signal_handler);
    (void)std::signal(SIGSEGV, signal_handler);
    (void)std::signal(SIGABRT, signal_handler);
}

auto main(int argc, char **argv) -> int {
    int opt{};
    std::string interface = "can0";

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
    } catch (const std::exception &myCustomException) {
        std::cout << myCustomException.what() << std::endl;
        return 1;
    }

    struct can_frame frame {};  // Classical can frame
    int32_t numBytes{};
    // Connect to zmq
    zmq::context_t ctx(1);
    zmq::socket_t publisher(ctx, zmq::socket_type::pub);
    publisher.bind("ipc:///tmp/speed.ipc");

    catch_signals();
    while (true) {
        numBytes = can->readMessage(&frame);

        if (numBytes < 0) {
            if (errno == EINTR) {
                std::cout << "interrupt received, exiting gracefully..." << std::endl;
                break;
            }
            if (errno != EAGAIN) {
                perror("Error reading");
            }
            continue;
        }

        // The len element contains the payload length in bytes and should be used instead of
        // can_dlc
        printf("0x%03X [%d] ", frame.can_id, frame.len);

        for (int i = 0; i < frame.len; i++) {
            std::cout << printf("%02X ", frame.data[i]);
        }

        std::cout << "\n";

        zmq::message_t msg{static_cast<size_t>(numBytes)};
        memcpy(msg.data(), frame.data, static_cast<size_t>(numBytes));

        try {
            publisher.send(msg, zmq::send_flags::none);
        } catch (zmq::error_t &e) {
            std::cout << "interrupt received, proceeding...\n";
        }

        if (static_cast<bool>(interrupted)) {
            std::cout << "interrupt received, killing program...\n";
            break;
        }
    }

    // Closing ZeroMQ
    std::cout << "Closing publisher\n";
    publisher.close();
    std::cout << "Closing context\n";
    ctx.close();

    return 0;
}
