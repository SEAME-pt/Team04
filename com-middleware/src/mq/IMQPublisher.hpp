#ifndef MQ_PUBLISHER_HPP
#define MQ_PUBLISHER_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace mq {

class IMQPublisher {
   protected:
    // Constructors
    IMQPublisher() = default;
    IMQPublisher(const IMQPublisher&) = default;
    IMQPublisher(IMQPublisher&&) = default;

    // Asignment operators
    auto operator=(const IMQPublisher&) -> IMQPublisher& = default;
    auto operator=(IMQPublisher&&) -> IMQPublisher& = default;

   public:
    virtual ~IMQPublisher() = default;
    virtual void subscribe(const std::string& topic) = 0;
    virtual auto publish(const std::vector<uint8_t>& data) -> bool = 0;
};
}  // namespace mq

#endif
