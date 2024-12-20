#include "rpm.h"

Rpm::Rpm(int initial) {
    this->value = initial;
}

Rpm::~Rpm() {}

void Rpm::setRPM(int rpm) {
    this->value = rpm;
}

int Rpm::getRPM() const {
    return this->value;
}
