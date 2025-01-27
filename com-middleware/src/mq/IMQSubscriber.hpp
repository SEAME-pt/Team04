#ifndef MQ_SUBSCRIBER_HPP
#define MQ_SUBSCRIBER_HPP

#include <string>
#include <vector>

namespace mq {

class IMQSubscriber {
   protected:
    // Constructors
    IMQSubscriber() = default;
    IMQSubscriber(const IMQSubscriber&) = default;
    IMQSubscriber(IMQSubscriber&&) = default;

    // Asignment operators
    auto operator=(const IMQSubscriber&) -> IMQSubscriber& = default;
    auto operator=(IMQSubscriber&&) -> IMQSubscriber& = default;

   public:
    virtual ~IMQSubscriber() = 0;
    virtual void subscribe(const std::string& topic) = 0;
    virtual auto receive() -> std::vector<uint8_t> = 0;
};
}  // namespace mq

#endif
