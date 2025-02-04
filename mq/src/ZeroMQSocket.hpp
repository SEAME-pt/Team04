#include "IMQSocket.hpp"
#include "zmq.hpp"

namespace MQ {
class ZeroMQSocket : public IMQSocket {
   private:
    zmq::socket_t m_socket;

   public:
    ZeroMQSocket(zmq::context_t& context, zmq::socket_type type) : m_socket(context, type) {}
    ~ZeroMQSocket() override = default;
    ZeroMQSocket(ZeroMQSocket&&) = default;

    auto connect(const std::string& endpoint) -> bool override;
    auto bind(const std::string& endpoint) -> bool override;
    auto send(const std::vector<uint8_t>& data) -> bool override;
    auto receive() -> std::vector<uint8_t> override;
    void close() override;
};
}  // namespace MQ
