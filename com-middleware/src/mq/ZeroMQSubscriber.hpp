#ifndef ZERO_MQ_SUBSCRIBER_HPP
#define ZERO_MQ_SUBSCRIBER_HPP

#include <memory>
#include <string>
#include <zmq.hpp>

#include "IMQSubscriber.hpp"

namespace mq {

class ZeroMQSubscriber : public IMQSubscriber {
   protected:
    // Constructors
    ZeroMQSubscriber() = default;
    ZeroMQSubscriber(ZeroMQSubscriber&&) = default;

    // Asignment operators
    auto operator=(ZeroMQSubscriber&&) -> ZeroMQSubscriber& = default;

   public:
    ~ZeroMQSubscriber();
    explicit ZeroMQSubscriber(const std::shared_ptr<zmq::context_t>& context);
    ZeroMQSubscriber(const ZeroMQSubscriber&) = delete;
    auto operator=(const ZeroMQSubscriber&) -> ZeroMQSubscriber& = delete;
    void subscribe(const std::string& topic) override;
    auto receive() -> std::vector<uint8_t> override;

   private:
    std::shared_ptr<zmq::context_t> m_context;
    zmq::socket_t m_socket;
};
}  // namespace mq

#endif
