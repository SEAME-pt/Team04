#include "ZeroMQSocket.hpp"

#include <iostream>

namespace MQ {
void ZeroMQSocket::close() {
    std::cout << "Closing socket\n";
    m_socket.close();
}

auto ZeroMQSocket::bind(const std::string& endpoint) -> bool {
    std::cout << "Binding to " << endpoint << "\n";
    try {
        m_socket.set(zmq::sockopt::linger, 0);
        m_socket.bind(endpoint);
        return true;
    } catch (const zmq::error_t& e) {
        std::cout << "Error binding to " << endpoint << ": " << e.what() << "\n";
        return false;
    }
}

auto ZeroMQSocket::subscribe(const std::string& topic) -> bool {
    std::cout << "Subscribing to \"" << topic << "\"\n";
    try {
        m_socket.set(zmq::sockopt::subscribe, topic);
        return true;
    } catch (const zmq::error_t& e) {
        std::cout << "Error subscribing to " << topic << ": " << e.what() << "\n";
        return false;
    }
}

auto ZeroMQSocket::connect(const std::string& endpoint) -> bool {
    std::cout << "Connecting to " << endpoint << "\n";
    try {
        m_socket.set(zmq::sockopt::linger, 0);
        m_socket.connect(endpoint);
        return true;
    } catch (const zmq::error_t& e) {
        std::cout << "Error connecting to " << endpoint << ": " << e.what() << "\n";
        return false;
    }
}

auto ZeroMQSocket::send(const std::vector<uint8_t>& data) -> bool {
    zmq::message_t msg(data.size());
    memcpy(msg.data(), data.data(), data.size());
    auto res = m_socket.send(msg, zmq::send_flags::none);
    return res.has_value();
}

auto ZeroMQSocket::receive() -> std::optional<std::vector<uint8_t>> {
    zmq::message_t msg;
    if (!m_socket.recv(msg, zmq::recv_flags::none)) {
        return {};
    }
    std::vector<uint8_t> data(msg.size());
    memcpy(data.data(), msg.data(), msg.size());
    return data;
}

}  // namespace MQ
