#ifndef MQ_SOCKET_HPP
#define MQ_SOCKET_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace MQ {

class IMQSocket {
   protected:
    // Constructors
    IMQSocket() = default;
    IMQSocket(const IMQSocket&) = default;
    IMQSocket(IMQSocket&&) = default;

    // Asignment operators
    auto operator=(const IMQSocket&) -> IMQSocket& = default;
    auto operator=(IMQSocket&&) -> IMQSocket& = default;

   public:
    virtual ~IMQSocket() = default;
    virtual auto connect(const std::string& endpoint) -> bool = 0;
    virtual auto bind(const std::string& endpoint) -> bool = 0;
    virtual auto send(const std::vector<uint8_t>& data) -> bool = 0;
    virtual auto receive() -> std::vector<uint8_t> = 0;
    virtual void close() = 0;
};
}  // namespace MQ

#endif
