#include "CanDriver.hpp"

#include <linux/can.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "com-middleware/exceptions/CanCloseException.hpp"
#include "com-middleware/exceptions/CanInitException.hpp"

namespace candriver {

CanDriver::CanDriver(std::string can_interface, int32_t timeout_s)
    : m_canInterface(std::move(can_interface)), m_timeout_s(timeout_s) {
    initializeCan();
};

void CanDriver::initializeCan() {
    if (m_canInterface.empty()) {
        throw exceptions::CanInitException("Can interface is empty");
    }
    struct sockaddr_can addr {};
    struct ifreq ifr {};  // interface

    // opening a socket for communicating over a CAN network
    // Since SocketCAN implements a new protocol family, we pass PF_CAN as the first argument to the
    // socket(2) system call there are two CAN protocols to choose from, the raw socket protocol
    // (SOCK_RAW) and the broadcast manager (CAN_BCM)
    m_can_socket_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (m_can_socket_fd < 0) {
        throw exceptions::CanInitException("Error connecting to socket");
    }

    // Setting the can interface
    strcpy(ifr.ifr_name, m_canInterface.c_str());
    // To determine the interface index an appropriate ioctl() has to be used (0 for all)
    ioctl(m_can_socket_fd, SIOCGIFINDEX, &ifr);

    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    // Timeout for read blocking
    struct timeval timeout {};
    timeout.tv_sec = m_timeout_s;  // Timeout in seconds
    timeout.tv_usec = 0;           // Microseconds
    setsockopt(m_can_socket_fd, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char *>(&timeout),
               sizeof(struct timeval));

    if (bind(m_can_socket_fd, reinterpret_cast<struct sockaddr *>(&addr), sizeof(addr)) < 0) {
        throw exceptions::CanInitException("Error binding socket");
    }
}

void CanDriver::uninitializeCan() const {
    std::cout << "Closing socket\n";
    if (close(m_can_socket_fd) < 0) {
        throw exceptions::CanCloseException("Error closing socket");
    }
}

auto CanDriver::readMessage(void *buffer) const -> int32_t {
    return static_cast<int32_t>(read(m_can_socket_fd, buffer, sizeof(struct can_frame)));
}
}  // namespace candriver