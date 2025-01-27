#ifndef CAN_DRIVER_HPP
#define CAN_DRIVER_HPP

#include <string>

namespace candriver {

class CanDriver {
   private:
    std::string m_canInterface;
    int32_t m_timeout;

   public:
    CanDriver(std::string can_interface, int32_t timeout);

    /**
     * @brief Initializes CAN socket
     *
     * A more detailed description of the method,
     * including its behavior and any important considerations.
     *
     * @throws CanInitException if it fails to initialize
     */
    void initializeCan();
};
}  // namespace candriver

#endif