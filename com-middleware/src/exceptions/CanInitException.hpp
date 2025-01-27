#ifndef CAN_DRIVER_EXCEPTIONS_CANINITEXCEPTION_HPP
#define CAN_DRIVER_EXCEPTIONS_CANINITEXCEPTION_HPP

#include <exception>
#include <string>
#include <utility>

namespace candriver::exceptions {

using std::exception;
using std::string;

/**
 * @brief An exception that may be thrown when an error occurred while initialising a CAN socket.
 */
class CanInitException : public exception {
   public:  // +++ Constructor / Destructor +++
    explicit CanInitException(string message) : _message(std::move(message)) {}
    virtual ~CanInitException() {}
    [[nodiscard]] auto what() const noexcept -> const char* override { return _message.c_str(); }

   private:
    string _message;
};

}  // namespace candriver::exceptions

#endif  // CAN_DRIVER_EXCEPTIONS_CANINITEXCEPTION_HPP
