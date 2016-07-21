#include "StepperDriver.h"

StepperDriver::StepperDriver(int stepPin, int dirPin, int enablePin) {
    this->stepPin = stepPin;
    this->dirPin = dirPin;
    this->enablePin = enablePin;
}

void StepperDriver::setup() {
    pinMode(stepPin,OUTPUT);
    pinMode(dirPin,OUTPUT);
    pinMode(enablePin,OUTPUT);
    digitalWrite(enablePin, HIGH); // Turn off steppers (HIGH)
}

void StepperDriver::enable() {

}

void StepperDriver::disable() {

}

void StepperDriver::setDirection (int dir) {

}
