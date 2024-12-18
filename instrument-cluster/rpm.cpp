#include "rpm.h"

RPM::RPM(int initial) {
    this->value = initial;
}

RPM::~RPM() {}

void RPM::setRPM(int rpm) {
    this->value = rpm;
}

int RPM::getRPM() const {
    return this->value;
}
