# Joystick Controller Documentation

## Overview
The Joystick Controller is a C++ implementation for handling game controller input using SDL2's GameController API. It provides a flexible interface for mapping controller buttons and axes to car control functions, integrating with the carMove system for controlling a robotic car.

## Dependencies
- SDL2 library with GameController support
- carMove class
- Standard C++ libraries (array, functional, iostream, map, utility)

## Class Reference

### joystick Class

#### Constructor
```cpp
joystick(carMove& car);
```
Initializes the joystick controller system:
1. Initializes SDL2 GameController subsystem
2. Scans for available game controllers
3. Opens the first compatible controller found
4. Sets up default axis mappings for the car
5. Throws `std::runtime_error` if:
   - SDL2 initialization fails
   - No compatible controller is found

#### Destructor
```cpp
~joystick();
```
Performs cleanup:
1. Closes the game controller connection
2. Shuts down SDL2

#### Public Methods

##### setButtonAction
```cpp
void setButtonAction(int button, Actions actions);
```
Maps a controller button to specific press and release actions
- Parameters:
  - `button`: Button identifier (SDL_GameControllerButton value)
  - `actions`: Structure containing:
    - `onPress`: Function to execute when button is pressed
    - `onRelease`: Function to execute when button is released

##### setAxisAction
```cpp
void setAxisAction(int axis, std::function<void(int)> action);
```
Maps a controller axis to a specific action
- Parameters:
  - `axis`: Axis identifier (SDL_GameControllerAxis value)
  - `action`: Function taking an integer value (-32768 to 32767)

##### setAxisMapping
```cpp
void setAxisMapping(carMove& car);
```
Sets up default axis mappings for car control:
1. Left stick X-axis (1): Controls steering angle (-45° to 45°)
2. Right stick Y-axis (5): Controls motor speed (-100% to 100%)

##### listen
```cpp
void listen();
```
Enters the main event processing loop:
1. Continuously polls for controller events
2. Processes button presses and axis movements
3. Exits when specific button combination is pressed (buttons 4 and 6)
4. Handles controller disconnection events

##### printButtonStates
```cpp
void printButtonStates();
```
- When a button is pressed, print its number in the console
- Use this method to map the control buttons

#### Private Methods

##### processEvent
```cpp
void processEvent(const SDL_Event& event);
```
Internal event processing method:
- Handles:
  - Button press/release events
  - Axis motion events
  - Controller disconnection events
- Updates button states
- Executes mapped actions
- Logs event information to console

## Data Structures

### Actions Structure
```cpp
struct Actions {
    std::function<void()> onPress;   // Function called on button press
    std::function<void()> onRelease; // Function called on button release
};
```

### Class Members
```cpp
SDL_GameController* gameController;  // Pointer to the active controller
std::map<int, Actions> buttonActions; // Maps buttons to their actions
std::map<int, std::function<void(int)>> axisActions; // Maps axes to their actions
std::array<bool, SDL_CONTROLLER_BUTTON_MAX> buttonStates; // Current button states
```

## Usage Example

```cpp
try {
    carMove car;
    joystick controller(car);
    
    // Custom button mapping example
    controller.setButtonAction(SDL_CONTROLLER_BUTTON_A, 
        Actions{
            []() { std::cout << "A pressed\n"; },
            []() { std::cout << "A released\n"; }
        });
    
    // Custom axis mapping example
    controller.setAxisAction(SDL_CONTROLLER_AXIS_LEFTX,
        [](int value) {
            std::cout << "Left stick X: " << value << "\n";
        });
    
    // Start processing controller input
    controller.listen();
    
} catch (const std::runtime_error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```

## Event Handling
The class handles several types of SDL events:
1. SDL_CONTROLLERBUTTONDOWN: Button press events
2. SDL_CONTROLLERBUTTONUP: Button release events
3. SDL_CONTROLLERAXISMOTION: Axis movement events
4. SDL_CONTROLLERDEVICEREMOVED: Controller disconnection events

## Default Mappings
- Left Stick X-Axis:
  - Range: -32768 to 32767
  - Mapped to steering angle: -45° to 45°
- Right Stick Y-Axis:
  - Range: -32768 to 32767
  - Mapped to motor speed: -100% to 100%

## Error Handling
- Throws `std::runtime_error` for initialization failures
- Handles controller disconnection gracefully
- Validates button and axis indices before processing
- Logs events and errors to standard output

## Safety Considerations
1. Always check for null controller pointer before operations
2. Handle controller disconnection appropriately
3. Implement emergency stop functionality
4. Validate axis values before applying to car controls
5. Consider implementing dead zones for axis inputs

## Maintenance Notes
- Regularly check SDL2 events for controller status
- Monitor controller connection stability
- Consider implementing controller reconnection handling
- Add support for multiple controller types/layouts
- Implement axis calibration functionality if needed
