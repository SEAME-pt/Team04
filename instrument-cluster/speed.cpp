#include "speed.h"

Speed::Speed(double initial) {
    this->value = initial;
}

Speed::~Speed() {}

void Speed::setSpeed(double speed) {
    this->value = speed;
}

double Speed::getSpeed() const {
    return this->value;
}
