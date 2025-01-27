#include <linux/can.h>

#include <cstdint>

/**
 * @brief Asbtracts the Linux CAN Socket logic
 *
 */
class ICanDriver {
   public:
    ICanDriver() = default;
    ICanDriver(const ICanDriver&) = default;
    auto operator=(const ICanDriver&) -> ICanDriver& = default;
    ICanDriver(ICanDriver&&) = default;
    auto operator=(ICanDriver&&) -> ICanDriver& = default;
    virtual ~ICanDriver() = default;

    /**
     * @brief Receives CAN message
     *
     * @param frame CAN frame
     * @return int32_t number of bytes received
     */
    virtual auto receive(can_frame* frame) const -> int32_t = 0;
};
