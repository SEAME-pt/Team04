# Main.cpp Documentation

## Overview
### Example Implementation: Raspberry Pi-Based Car Control System
This project showcases a Raspberry Pi-based car control system that combines manual driving via a game controller with autonomous driving capabilities. The system leverages network sockets to enable external systems to send driving instructions for autonomous operation. Additionally, a Jetson Nano board is integrated to process images, execute machine learning models, and run Model Predictive Control (MPC) algorithms. The Jetson Nano communicates with the Raspberry Pi to relay driving commands, demonstrating a seamless blend of hardware and software for real-time vehicle management and advanced autonomous functionality.

## Code Structure

```cpp
#include "platform/joystick/joystick.hpp"

auto main() -> int {
    try {
        CarMove car;
        Joystick controller(car);
        controller.listen();

    } catch (const std::runtime_error &e) {
        std::cerr << "Error: " << e.what() << '\n';
    }

    return 0;
}
```

## Dependencies

- **platform/joystick/joystick.hpp**: Main joystick controller interface
- **Implicit dependencies** through `Joystick` class:
  - `CarMove`: Car movement control system
  - `RemoteMove`: Remote autonomous driving capabilities
  - SDL2: Game controller support
  - ZeroMQ: Network communication for remote control

## Function Analysis

### `main()` Function

**Return Type**: `int` (using modern C++ trailing return type syntax)

**Purpose**: Initializes the car control system and starts the game controller listener.

**Execution Flow**:

1. **Car Initialization**
   ```cpp
   CarMove car;
   ```
   - Creates a `CarMove` instance to handle physical car control
   - Initializes motor and servo control systems

2. **Controller Setup**
   ```cpp
   Joystick controller(car);
   ```
   - Creates a `Joystick` instance linked to the car object
   - Initializes SDL2 game controller subsystem
   - Scans for and connects to available game controllers
   - Sets up default button and axis mappings
   - Creates remote control server (TCP port 5555)

3. **Event Loop Start**
   ```cpp
   controller.listen();
   ```
   - Enters the main event processing loop
   - Continuously monitors for controller input
   - Processes button presses, releases, and axis movements
   - Handles autonomous driving mode switching
   - **Blocking operation**: Program execution stays in this loop until exit conditions are met

## Program Flow

```
Start
  ↓
Initialize CarMove
  ↓
Initialize Joystick Controller
  ├─ Initialize SDL2
  ├─ Connect to Game Controller
  ├─ Setup Button/Axis Mappings
  └─ Start Remote Control Server
  ↓
Enter Controller Event Loop
  ├─ Process Button Events
  ├─ Process Axis Events
  ├─ Handle Autonomous Mode
  └─ Check Exit Conditions
  ↓
Exit (Buttons 4+6 or Controller Disconnect)
  ├─ Stop Remote Control
  ├─ Cleanup SDL2 Resources
  └─ Return 0
```

## Exit Conditions

The program terminates under these conditions:

1. **Manual Exit**: Pressing buttons 4 and 6 simultaneously on the controller
2. **Controller Disconnection**: Automatic termination when controller is unplugged
3. **Initialization Error**: Exception during startup (returns to OS)

## Features Enabled

Through the integrated classes, this main function enables:

### Manual Control
- **Steering**: Left analog stick horizontal axis
- **Acceleration/Braking**: Right trigger
- **Real-time response**: Direct controller-to-car communication

### Autonomous Driving
- **Enable**: Button 1 activates remote autonomous mode
- **Disable**: Button 3 deactivates autonomous mode and returns to manual
- **Network Interface**: TCP server on port 5555 for external control

### System Management
- **Status Display**: Button configuration and current states
- **Graceful Shutdown**: Proper resource cleanup on exit
- **Error Recovery**: Exception handling for initialization failures

## Usage Instructions

### Prerequisites
- Game controller connected via USB or Bluetooth
- Proper SDL2 installation and configuration
- Car hardware properly connected and configured

### Running the Application
```bash
# Compile and run
./car_control_app
```

### Controller Layout
- **Button 1**: Enable autonomous driving
- **Button 3**: Disable autonomous driving
- **Buttons 4+6**: Exit application (press together)
- **Left Stick**: Steering control
- **Right Trigger**: Speed control

### Remote Control
Once running, external systems can connect to:
- **Address**: `tcp://localhost:5555`
- **Protocol**: Send 8-byte messages with two float values
- **Format**: `[acceleration: float, steering: float]` (range: -1.0 to 1.0)

## Architecture Benefits

This main function demonstrates several good software practices:

1. **Separation of Concerns**: Each class handles a specific responsibility
2. **Exception Safety**: Proper error handling for initialization failures  
3. **Resource Management**: Automatic cleanup through destructors
4. **Modularity**: Easy to extend with additional control methods
5. **Simplicity**: Clean, readable main function with clear purpose