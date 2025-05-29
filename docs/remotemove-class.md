# RemoteMove Class Documentation

## Overview

The `RemoteMove` class provides remote control functionality for car movement using ZeroMQ (ZMQ) messaging. It creates a server that listens for remote commands containing acceleration and steering data, then applies these commands to a car control system. This enables autonomous or remote control of the vehicle over a network connection.

## Dependencies

- **zmq.hpp**: ZeroMQ C++ bindings for network communication
- **CarMove**: Car movement control interface
- **Standard Libraries**: atomic, iostream, string, thread, chrono, cstring, stdexcept

## Class Structure

### Communication Protocol

The class uses a **request-reply (REP)** ZeroMQ socket pattern:
- **Input**: 8-byte messages containing two float values (acceleration, steering)
- **Output**: "OK" for successful commands, "ERROR" for invalid messages
- **Value Ranges**: Both acceleration and steering are clamped to [-1.0, 1.0]

## Constructor

### `RemoteMove(CarMove& car_instance, std::string bind_address)`

**Purpose**: Initializes the remote control server and binds to the specified network address.

**Parameters**:
- `car_instance`: Reference to a `CarMove` object that will receive the remote commands
- `bind_address`: ZeroMQ socket address to bind to (e.g., "tcp://0.0.0.0:5555", "tcp://*:5555")

**Functionality**:
- Creates a ZeroMQ context with 1 I/O thread
- Creates a REP (reply) socket for request-response communication
- Binds the socket to the specified address
- Initializes the car reference and sets running state to false
- Prints confirmation message with the bound address

**Example Addresses**:
```cpp
RemoteMove remote(car, "tcp://*:5555");        // Listen on all interfaces, port 5555
RemoteMove remote(car, "tcp://127.0.0.1:5555"); // Listen on localhost only
RemoteMove remote(car, "ipc:///tmp/car_control"); // Unix domain socket
```

## Destructor

### `~RemoteMove()`

**Purpose**: Ensures proper cleanup when the object is destroyed.

**Functionality**:
- Calls `stop()` method to gracefully shut down the listener thread
- ZeroMQ context and socket are automatically cleaned up by their destructors

## Public Methods

### `void start()`

**Purpose**: Starts the remote control listener in a separate thread.

**Functionality**:
- Checks if the listener is not already running
- Sets the atomic `running` flag to `true`
- Creates and starts a new thread running `listenerLoop()`
- Prints confirmation message
- **Thread Safety**: Uses atomic boolean to prevent multiple simultaneous starts

**Usage**:
```cpp
RemoteMove remote(car, "tcp://*:5555");
remote.start(); // Begin listening for remote commands
```

### `void stop()`

**Purpose**: Gracefully stops the remote control listener and cleans up the thread.

**Functionality**:
- Checks if the listener is currently running
- Sets the atomic `running` flag to `false`
- Waits for the listener thread to finish using `join()`
- Prints confirmation message
- **Thread Safety**: Ensures clean shutdown without race conditions

**Usage**:
```cpp
remote.stop(); // Stop listening and clean up resources
```

## Private Methods

### `void listenerLoop()`

**Purpose**: Main loop that runs in a separate thread to handle incoming remote commands.

**Functionality**:
- Runs continuously while `running` is `true`
- Uses non-blocking receive (`zmq::recv_flags::dontwait`) to check for messages
- Validates message size (must be exactly 8 bytes for two floats)
- Extracts and clamps acceleration and steering values to [-1.0, 1.0] range
- Processes valid commands and sends "OK" response
- Sends "ERROR" response for invalid message sizes
- Includes 10ms sleep to prevent excessive CPU usage

**Message Validation**:
```cpp
// Expected message format:
struct Command {
    float acceleration; // -1.0 to 1.0
    float steering;     // -1.0 to 1.0
};
```

**Error Handling**:
- Invalid message sizes trigger error responses
- Values outside [-1.0, 1.0] are automatically clamped
- Non-blocking receive prevents thread blocking

### `void processCommand(float acceleration, float steering)`

**Purpose**: Converts normalized remote commands to car-specific control values.

**Parameters**:
- `acceleration`: Normalized acceleration value (-1.0 to 1.0)
- `steering`: Normalized steering value (-1.0 to 1.0)

**Conversion Logic**:
- **Acceleration**: Maps [-1.0, 1.0] to motor speed [-100, 100] (percentage)
- **Steering**: Maps [-1.0, 1.0] to servo angle [-45°, 45°]

**Functionality**:
- Converts float values to appropriate integer ranges
- Logs the received command with both normalized and converted values
- Applies commands to the car using `setMotorSpeed()` and `setServoAngle()`

**Example**:
```cpp
// Input: acceleration = 0.5, steering = -0.3
// Output: motorSpeed = 50%, servoAngle = -13.5° (rounded to -13°)
```

## Private Members

### Network Communication
- `zmq::context_t context`: ZeroMQ context managing I/O threads
- `zmq::socket_t socket`: REP socket for request-reply communication
- `std::string address`: Bound network address

### Control Integration
- `CarMove& car`: Reference to the car control object

### Thread Management
- `std::atomic<bool> running`: Thread-safe flag controlling the listener loop
- `std::thread listener_thread`: Background thread handling network messages

## Communication Flow

1. **Client sends 8-byte message** containing two float values
2. **Server receives and validates** message size
3. **Values are extracted and clamped** to valid ranges
4. **Commands are processed** and applied to car
5. **Response is sent back**:
   - "OK" for successful processing
   - "ERROR" for invalid messages

## Usage Example

```cpp
// Initialize car control
CarMove car;

// Create remote control server
RemoteMove remote(car, "tcp://*:5555");

// Start listening for commands
remote.start();

// ... system runs, handling remote commands ...

// Stop when done
remote.stop();
```

## Client Example (Python with PyZMQ)

```python
import zmq
import struct

context = zmq.Context()
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:5555")

# Send command: 50% forward, 30% left turn
acceleration = 0.5
steering = -0.3
message = struct.pack('ff', acceleration, steering)
socket.send(message)

# Receive response
response = socket.recv()
print(response.decode())  # Should print "OK"
```

## Value Ranges and Conversions

| Input Range | Parameter | Output Range | Car Command |
|-------------|-----------|--------------|-------------|
| -1.0 to 1.0 | acceleration | -100 to 100 | Motor speed (%) |
| -1.0 to 1.0 | steering | -45° to 45° | Servo angle |

## Thread Safety

- Uses `std::atomic<bool>` for the running flag
- ZeroMQ handles internal thread safety
- Proper thread joining ensures clean shutdown
- Non-blocking operations prevent deadlocks

## Error Handling

- **Invalid message size**: Sends "ERROR" response
- **Network errors**: ZeroMQ handles connection issues automatically  
- **Value clamping**: Out-of-range values are automatically constrained
- **Thread safety**: Atomic operations prevent race conditions

## Performance Considerations

- **10ms loop delay**: Balances responsiveness with CPU usage
- **Non-blocking receive**: Prevents thread blocking on no messages
- **Single I/O thread**: Sufficient for typical car control applications
- **Direct memory access**: Efficient float extraction from messages