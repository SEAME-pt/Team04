#include "ZeroMQPublisher.hpp"

#include <iostream>

namespace mq {
ZeroMQPublisher::ZeroMQPublisher(std::unique_ptr<zmq::socket_t> socket)
    : m_socket(std::move(socket)) {};

ZeroMQPublisher::~ZeroMQPublisher() {
    if (m_socket) {
        std::cout << "Closing publisher\n";
        m_socket->close();
    }
}

void ZeroMQPublisher::subscribe(const std::string& topic) { m_socket->bind(topic); }

auto ZeroMQPublisher::publish(const std::vector<uint8_t>& data) -> bool {
    zmq::message_t msg(data.size());
    memcpy(msg.data(), data.data(), data.size());
    std::cout << m_socket.get() << "\n";
    zmq::send_result_t res = m_socket->send(msg, zmq::send_flags::none);
    if (res.has_value()) {
        std::cout << "!sent\n";
        return true;
    }
    return false;
}
}  // namespace mq
