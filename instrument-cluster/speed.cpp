#include "speed.h"

Speed::Speed(double initial) : value(initial) {
    
}

Speed::~Speed() = default;

void Speed::setSpeed(double speed) {
    this->value = speed;
}

auto Speed::getSpeed() const -> double {
    return this->value;
}
