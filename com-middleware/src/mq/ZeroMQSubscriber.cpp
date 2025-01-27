#include "ZeroMQSubscriber.hpp"

namespace mq {
ZeroMQSubscriber::ZeroMQSubscriber(const std::shared_ptr<zmq::context_t>& context)
    : m_context(context), m_socket(*context, zmq::socket_type::sub) {};

void ZeroMQSubscriber::subscribe(const std::string& topic) { m_socket.bind(topic); }

auto ZeroMQSubscriber::receive() -> std::vector<uint8_t> {
    zmq::message_t msg;
    if (!m_socket.recv(msg, zmq::recv_flags::none)) {
        return {};
    }

    std::vector<uint8_t> data(msg.size());
    memcpy(data.data(), msg.data(), msg.size());
    return data;
}
}  // namespace mq
