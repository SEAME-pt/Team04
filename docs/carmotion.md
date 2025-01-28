# Car Movement System Documentation

## Overview
The Car Movement System is a C++ implementation for controlling a robotic car's movement through servo motors and motor drivers using I2C communication. The system provides control over steering angle and motor speed, integrating with the pigpio library for GPIO control on Raspberry Pi.

## Dependencies
- pigpio library for GPIO control
- I2C enabled on the Raspberry Pi
- Standard C++ libraries (algorithm, cmath, stdexcept)

## Hardware Configuration
### Servo Configuration
- I2C Address: 0x40
- PWM Frequency: ~50Hz
- Steering Channel: 0
- PWM Values:
  - Left Maximum (45°): 205
  - Center (0°): 307
  - Right Maximum (45°): 410

### Motor Configuration
- I2C Address: 0x60
- PWM Frequency: 60Hz
- Channels:
  - ENA: Channel 0
  - IN1: Channel 1
  - IN2: Channel 2
  - IN3: Channel 5
  - IN4: Channel 6
  - ENB: Channel 7

## Class Reference

### carMove Class

#### Constructor
```cpp
carMove();
```
Initializes the car movement system by:
1. Initializing the pigpio library
2. Setting up I2C communication for both servo and motor controllers
3. Configuring PWM frequencies and modes
4. Throws `std::runtime_error` if initialization fails

#### Destructor
```cpp
~carMove();
```
Performs cleanup by:
1. Closing I2C connections
2. Terminating the pigpio library

#### Public Methods

##### setServoAngle
```cpp
void setServoAngle(int angle);
```
Sets the steering angle of the car.
- Parameters:
  - `angle`: Integer value between -45 and 45 degrees
    - Negative values: Turn left
    - Positive values: Turn right
    - Zero: Center position
- Automatically clamps values to valid range

##### setMotorSpeed
```cpp
void setMotorSpeed(int speed);
```
Controls the car's motor speed and direction
- Parameters:
  - `speed`: Integer value between -100 and 100
    - Positive values: Forward motion
    - Negative values: Backward motion
    - Zero: Stop
- Automatically clamps values to valid range

##### sequence
```cpp
void sequence();
```
Demonstrates basic movement capabilities:
1. Drives forward at full speed (2 seconds)
2. Drives backward at full speed (2 seconds)
3. Turns wheels fully left (1 second)
4. Turns wheels fully right (1 second)
5. Returns to neutral position

#### Private Methods

##### setPWM
```cpp
static void setPWM(int device_handle, int channel, int on_value, int off_value);
```
Low-level PWM control for both servo and motor
- Parameters:
  - `device_handle`: I2C device handle
  - `channel`: PWM channel number
  - `on_value`: PWM signal start time
  - `off_value`: PWM signal stop time

## Constants

```cpp
static const int SERVO_ADDR = 0x40;      // Servo controller I2C address
static const int MOTOR_ADDR = 0x60;      // Motor controller I2C address
static const int STEERING_CHANNEL = 0;   // Servo channel for steering
static constexpr int MAX_ANGLE = 45;     // Maximum steering angle
static const int SERVO_LEFT_PWM = 205;   // PWM value for maximum left turn
static const int SERVO_CENTER_PWM = 307; // PWM value for center position
static const int SERVO_RIGHT_PWM = 410;  // PWM value for maximum right turn
```

## Usage Example

```cpp
try {
    carMove car;
    
    // Basic movement
    car.setMotorSpeed(50);     // Move forward at half speed
    
    sleep(2);                  // Continue for 2 seconds
    
    car.setServoAngle(-30);    // Turn left at 30 degrees
    
    sleep(1);

    car.setMotorSpeed(0);      // Stop
    car.setServoAngle(0);      // Center wheels
    
    // Run demo sequence
    // car.sequence();
    
} catch (const std::runtime_error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```

## Error Handling
The class uses exception handling to manage errors:
- Throws `std::runtime_error` if:
  - pigpio initialization fails
  - I2C device initialization fails
- All motor and servo commands are bounds-checked to prevent invalid values


