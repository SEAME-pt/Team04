#ifndef ZERO_MQ_WORKER_HPP
#define ZERO_MQ_WORKER_HPP

#include <memory>
#include <string>
#include <zmq.hpp>

#include "IMQSocket.hpp"

namespace MQ {

class ZeroMQWorker {
   protected:
    // Constructors
    ZeroMQWorker() = default;
    ZeroMQWorker(ZeroMQWorker&&) = default;

    // Asignment operators
    auto operator=(ZeroMQWorker&&) -> ZeroMQWorker& = default;

   public:
    ~ZeroMQWorker();
    explicit ZeroMQWorker(std::unique_ptr<IMQSocket> socket);
    ZeroMQWorker(const ZeroMQWorker&) = delete;
    auto operator=(const ZeroMQWorker&) -> ZeroMQWorker& = delete;
    void bind(const std::string& endpoint);
    auto receive() -> std::optional<std::vector<uint8_t>>;
    auto publish(const std::vector<uint8_t>& data) -> bool;

   private:
    std::unique_ptr<IMQSocket> m_socket;
};
}  // namespace MQ

#endif
