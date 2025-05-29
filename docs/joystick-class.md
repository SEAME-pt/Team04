# Joystick Class Documentation

## Overview

The `Joystick` class provides an interface for handling game controller input using SDL2. It manages controller connections, button and axis mappings, and integrates with car movement control and remote autonomous driving functionality.

## Dependencies

- **SDL2/SDL.h**: Core SDL2 functionality
- **SDL2/SDL_gamecontroller.h**: Game controller support
- **CarMove**: Car movement control interface
- **RemoteMove**: Remote autonomous driving control

## Class Structure

### Actions Struct

```cpp
struct Actions {
    std::function<void()> onPress;
    std::function<void()> onRelease;
};
```

A utility structure that holds callback functions for button press and release events.

## Constructor

### `Joystick(CarMove& car)`

**Purpose**: Initializes the joystick controller and sets up the connection with the car control system.

**Parameters**:
- `car`: Reference to a `CarMove` object for controlling car movement

**Functionality**:
- Initializes SDL2 GameController subsystem
- Creates a `RemoteMove` instance for autonomous driving (TCP connection on port 5555)
- Scans for available game controllers and connects to the first one found
- Sets up default axis mappings for steering and motor control
- Configures default button actions:
  - Button 1: Enable autonomous driving
  - Button 3: Disable autonomous driving
- Displays controller information and button setup

**Exceptions**:
- Throws `std::runtime_error` if SDL2 GameController initialization fails

## Destructor

### `~Joystick()`

**Purpose**: Properly cleans up resources when the joystick object is destroyed.

**Functionality**:
- Deletes the `RemoteMove` instance
- Closes the game controller connection if active
- Shuts down SDL2 subsystem

## Public Methods

### `void setButtonAction(int button, Actions actions)`

**Purpose**: Assigns custom actions to a specific controller button.

**Parameters**:
- `button`: Button number/ID to configure
- `actions`: `Actions` struct containing press and release callback functions

**Usage**: Allows customization of button behavior by defining what happens when a button is pressed or released.

### `void setAxisAction(int axis, std::function<void(int)> action)`

**Purpose**: Assigns a custom action to a controller axis (analog sticks, triggers).

**Parameters**:
- `axis`: Axis number/ID to configure
- `action`: Function that takes an integer value and performs an action

**Usage**: Enables custom responses to analog input changes, such as steering or throttle control.

### `void setAxisMapping(CarMove& car)`

**Purpose**: Establishes default axis mappings for car control.

**Parameters**:
- `car`: Reference to the `CarMove` object

**Default Mappings**:
- **Axis 0** (Left stick horizontal): Steering control
  - Maps axis values (-32000 to +32000) to servo angles (-55° to +55°)
- **Axis 5** (Right trigger): Motor speed control
  - Processes trigger input and maps to motor speed values

### `void listen()`

**Purpose**: Main event loop that continuously processes controller input.

**Functionality**:
- Polls for SDL events in an infinite loop
- Processes controller button presses, releases, and axis movements
- Handles controller disconnection events
- Implements exit condition (Buttons 4+6 pressed simultaneously)
- Performs cleanup when exiting
- Includes a 10ms delay to prevent excessive CPU usage

**Exit Conditions**:
- Buttons 4 and 6 pressed together: Clean shutdown
- Controller disconnection: Immediate program termination

### `void printButtonStates()`

**Purpose**: Displays the current controller configuration and button states.

**Output**:
- Shows button mapping information
- Lists currently pressed buttons
- Provides user guidance for controller usage

## Private Methods

### `void processEvent(const SDL_Event& event)`

**Purpose**: Handles individual SDL events from the controller.

**Event Types Handled**:
- **SDL_CONTROLLERBUTTONDOWN/UP**: Button press and release events
- **SDL_CONTROLLERAXISMOTION**: Analog stick and trigger movements
- **SDL_CONTROLLERDEVICEREMOVED**: Controller disconnection

**Functionality**:
- Updates button state tracking
- Executes configured button actions
- Processes axis movements through registered callbacks
- Handles controller disconnection gracefully

## Private Members

### Controller Management
- `SDL_GameController* gameController`: Pointer to the active controller
- `CarMove* car_ref`: Reference to the car control object
- `RemoteMove* remote`: Pointer to remote autonomous driving controller

### Input Mapping
- `std::map<int, Actions> buttonActions`: Maps button IDs to action callbacks
- `std::map<int, std::function<void(int)>> axisActions`: Maps axis IDs to movement callbacks
- `std::array<bool, SDL_CONTROLLER_BUTTON_MAX> buttonStates`: Tracks current button states

### Shutdown Control (Unused in Current Implementation)
- `int shutdownButton`: Button designated for shutdown (default: 7)
- `std::chrono::steady_clock::time_point shutdownButtonPressTime`: Timestamp for shutdown timing
- `bool isShutdownButtonHeld`: Flag for shutdown button state
- `const std::chrono::seconds shutdownHoldDuration`: Required hold duration for shutdown

## Usage Example

```cpp
// Create car control object
CarMove car;

// Initialize joystick controller
Joystick joystick(car);

// Add custom button action
joystick.setButtonAction(2, Actions{
    []() { std::cout << "Button 2 pressed!\n"; },
    []() { std::cout << "Button 2 released!\n"; }
});

// Start listening for input
joystick.listen();
```

## Default Controller Layout

- **Button 1**: Enable autonomous driving
- **Button 3**: Disable autonomous driving  
- **Buttons 4+6**: Exit program (when pressed together)
- **Left Stick (Axis 0)**: Steering control
- **Right Trigger (Axis 5)**: Motor speed control

## Error Handling

- SDL2 initialization failures throw runtime exceptions
- Controller disconnection triggers immediate program exit
- Resource cleanup is handled automatically in the destructor