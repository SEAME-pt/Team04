#ifndef CAR_MOVE_HPP
#define CAR_MOVE_HPP

#include <pigpio.h>
#include <unistd.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

class carMove {
   public:
    carMove();
    ~carMove();

    void setMotorSpeed(int speed);
    void setServoAngle(int angle);

    void sequence();

    int servo_handle;
    int motor_handle;

    static void setPWM(int device_handle, int channel, int on_value, int off_value);

    // Constants
    static const int SERVO_ADDR = 0x40;
    static const int MOTOR_ADDR = 0x60;
    static const int STEERING_CHANNEL = 0;

    // The constant MAX_ANGLE is 'constexpr' to allow direct use at compile time
    static constexpr int MAX_ANGLE = 80;

    static const int SERVO_LEFT_PWM = 100;  // Adjust as needed >> TEST!!
    static const int SERVO_CENTER_PWM = 300;
    static const int SERVO_RIGHT_PWM = 550;
};

#endif
