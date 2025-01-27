#include "CanDriver.hpp"

#include "com-middleware/exceptions/CanInitException.hpp"

namespace candriver {

CanDriver::CanDriver(std::string can_interface, int32_t timeout)
    : m_canInterface(std::move(can_interface)), m_timeout(timeout) {};

void CanDriver::initializeCan() {
    if (m_canInterface.empty()) {
        throw exceptions::CanInitException("Can interface is empty");
    }
}
}  // namespace candriver