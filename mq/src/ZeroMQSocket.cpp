#include "ZeroMQSocket.hpp"

#include <iostream>

namespace MQ {
void ZeroMQSocket::close() {
    std::cout << "Closing socket\n";
    m_socket.close();
}

auto ZeroMQSocket::bind(const std::string& endpoint) -> bool {
    std::cout << "Binding to " << endpoint << "\n";
    m_socket.bind(endpoint);
    return true;
}

auto ZeroMQSocket::send(const std::vector<uint8_t>& data) -> bool {
    zmq::message_t msg(data.size());
    memcpy(msg.data(), data.data(), data.size());
    auto res = m_socket.send(msg, zmq::send_flags::none);
    if (res.has_value()) {
        std::cout << "!sent\n";
        return true;
    }
    return false;
}

auto ZeroMQSocket::receive() -> std::vector<uint8_t> {
    zmq::message_t msg;
    if (!m_socket.recv(msg, zmq::recv_flags::none)) {
        return {};
    }

    std::vector<uint8_t> data(msg.size());
    memcpy(data.data(), msg.data(), msg.size());
    return data;
}

}  // namespace MQ
