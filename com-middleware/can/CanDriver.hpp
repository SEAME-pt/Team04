#ifndef CAN_DRIVER_HPP
#define CAN_DRIVER_HPP

#include <string>

namespace candriver {

class CanDriver {
   private:
    std::string m_canInterface;
    int32_t m_timeout_s;
    int32_t m_can_socket_fd{};

   public:
    CanDriver(std::string can_interface, int32_t timeout_s);
    virtual ~CanDriver() noexcept(false) { uninitializeCan(); }

   private:
    /**
     * @brief Initializes CAN socket
     *
     * A more detailed description of the method,
     * including its behavior and any important considerations.
     *
     * @throws CanInitException if it fails to initialize
     */
    void initializeCan();
    void uninitializeCan() const;

   public:
    auto readMessage(void* buffer) const -> int32_t;
};
}  // namespace candriver

#endif