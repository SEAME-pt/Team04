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

    try {
        candriver::CanDriver can{interface, 1};
        can.initializeCan();
    } catch (const std::exception &myCustomException) {
        std::cout << myCustomException.what() << std::endl;
    }

    return 0;

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

    int can_socket{};
    int numBytes{};
    struct sockaddr_can addr {};
    struct ifreq ifr {};        // interface
    struct can_frame frame {};  // Classical can frame

    std::cout << "CAN Sockets Receive Demo\r\n";

    // opening a socket for communicating over a CAN network
    // Since SocketCAN implements a new protocol family, we pass PF_CAN as the first argument to the
    // socket(2) system call there are two CAN protocols to choose from, the raw socket protocol
    // (SOCK_RAW) and the broadcast manager (CAN_BCM)
    can_socket = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (can_socket < 0) {
        perror("Error connecting to Socket");
        return 1;
    }

    // Setting the can interface
    strcpy(ifr.ifr_name, interface.c_str());
    // To determine the interface index an appropriate ioctl() has to be used (0 for all)
    ioctl(can_socket, SIOCGIFINDEX, &ifr);

    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    // Timeout for read blocking
    struct timeval timeout {};
    timeout.tv_sec = 5;   // Timeout in seconds
    timeout.tv_usec = 0;  // Microseconds
    setsockopt(can_socket, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char *>(&timeout),
               sizeof(struct timeval));

    if (bind(can_socket, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0) {
        perror("Error Binding");
        return 1;
    }

    // Connect to zmq
    zmq::context_t ctx(1);
    zmq::socket_t publisher(ctx, zmq::socket_type::pub);
    publisher.bind("ipc:///tmp/speed.ipc");

    catch_signals();
    while (true) {
        numBytes = static_cast<int>(read(can_socket, &frame, sizeof(struct can_frame)));

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

    std::cout << "Closing socket\n";
    if (close(can_socket) < 0) {
        perror("Error closing can socket");
        return 1;
    }

    return 0;
}
