#ifndef CAR_MOVE_HPP
#define CAR_MOVE_HPP

#include <pigpio.h>
#include <unistd.h>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

class CarMove {
   public:
    CarMove();
    ~CarMove();

    void setMotorSpeed(int speed);
    void setServoAngle(int angle);
    void sequence();
    static void setPWM(int device_handle, int channel, int on_value, int off_value);

    static const int SERVO_ADDR = 0x40;
    static const int MOTOR_ADDR = 0x60;
    static const int STEERING_CHANNEL = 0;
    static constexpr int MAX_ANGLE = 80;
    static const int SERVO_LEFT_PWM = 100;
    static const int SERVO_CENTER_PWM = 300;
    static const int SERVO_RIGHT_PWM = 550;

   private:
    int servo_handle;
    int motor_handle;
};

#endif
