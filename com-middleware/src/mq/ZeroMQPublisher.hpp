#ifndef ZERO_MQ_PUBLISHER_HPP
#define ZERO_MQ_PUBLISHER_HPP

#include <memory>
#include <string>
#include <zmq.hpp>

#include "IMQPublisher.hpp"

namespace mq {

class ZeroMQPublisher : public IMQPublisher {
   protected:
    // Constructors
    ZeroMQPublisher() = default;
    ZeroMQPublisher(ZeroMQPublisher&&) = default;

    // Asignment operators
    auto operator=(ZeroMQPublisher&&) -> ZeroMQPublisher& = default;

   public:
    virtual ~ZeroMQPublisher() override;
    explicit ZeroMQPublisher(std::unique_ptr<zmq::socket_t> socket);
    ZeroMQPublisher(const ZeroMQPublisher&) = delete;
    auto operator=(const ZeroMQPublisher&) -> ZeroMQPublisher& = delete;
    void subscribe(const std::string& topic) override;
    auto publish(const std::vector<uint8_t>& data) -> bool override;

   private:
    std::unique_ptr<zmq::socket_t> m_socket;
};
}  // namespace mq

#endif
