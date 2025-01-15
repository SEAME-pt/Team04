#include "rpm.h"

Rpm::Rpm(int initial) : value(initial) {
    
}



void Rpm::setRPM(int rpm) {
    this->value = rpm;
}

auto Rpm::getRPM() const -> int {
    return this->value;
}
