#ifndef CAN_DRIVER_HPP
#define CAN_DRIVER_HPP

#include <linux/can.h>

#include <string>

#include "ICanDriver.hpp"

namespace candriver {

class CanDriver : public ICanDriver {
   private:
    std::string m_canInterface;
    int32_t m_timeout_s;
    int32_t m_can_socket_fd{};

   public:
    CanDriver(std::string can_interface, int32_t timeout_s);
    ~CanDriver() override;
    CanDriver() = delete;
    CanDriver(const CanDriver&) = default;
    auto operator=(const CanDriver&) -> CanDriver& = default;
    CanDriver(CanDriver&&) = default;
    auto operator=(CanDriver&&) -> CanDriver& = default;

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
    auto receive(can_frame* frame) const -> int32_t override;
};
}  // namespace candriver

#endif
