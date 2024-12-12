#include "speed.h"
Speed::Speed(double initial) {this->current = initial;}
void Speed::increase(){
    this->current+=0.1;
}
void Speed::decrease(){
    this->current-=0.1;
}
double Speed::getCurrent(){
    return this->current;
}
