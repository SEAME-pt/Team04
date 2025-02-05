#include "ZeroMQWorker.hpp"

#include <iostream>
#include <zmq.hpp>

namespace MQ {
ZeroMQWorker::ZeroMQWorker(std::unique_ptr<IMQSocket> socket) : m_socket(std::move(socket)) {};

ZeroMQWorker::~ZeroMQWorker() {
    if (m_socket) {
        std::cout << "Closing publisher\n";
        m_socket->close();
    }
}

void ZeroMQWorker::subscribe(const std::string& topic) { m_socket->bind(topic); }

auto ZeroMQWorker::publish(const std::vector<uint8_t>& data) -> bool {
    return m_socket->send(data);
}

auto ZeroMQWorker::receive() -> std::optional<std::vector<uint8_t>> { return m_socket->receive(); }
}  // namespace MQ
