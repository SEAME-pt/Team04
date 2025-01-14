#include "rpm.h"

Rpm::Rpm(int initial) : value(initial) {
    
}

Rpm::~Rpm() = default;

void Rpm::setRPM(int rpm) {
    this->value = rpm;
}

auto Rpm::getRPM() const -> int {
    return this->value;
}
