#include "StepperDriver.h"

/**
Very simple stepper controller class. It's not using the normal time tracking based
delay that many robot controllers use for controlling multiple axes at the same time
because of the design of the blaster. Only one axis can be moving at a time due
to mechanical constraints, so using delay() and delayMicroseconds() between step
pulses works fine.
*/

StepperDriver::StepperDriver() {

}

void StepperDriver::setup(int stepPin, int dirPin, int enablePin) {
    this->stepPin = stepPin;
    this->dirPin = dirPin;
    this->enablePin = enablePin;

    pinMode(stepPin,OUTPUT);
    pinMode(dirPin,OUTPUT);
    pinMode(enablePin,OUTPUT);
    digitalWrite(dirPin, LOW);
    digitalWrite(enablePin, HIGH); // Turn off steppers (HIGH)
    enabled = false;
    direction = 1;
}

void StepperDriver::enable() {
    enabled = true;
    digitalWrite(enablePin,LOW);
}

void StepperDriver::disable() {
    enabled = false;
    digitalWrite(enablePin, HIGH);
}
void StepperDriver::move(bool dir, int steps, int stepsPerSecond = 1000) {
    unsigned long microSecondsPerStep = ((1/(float)stepsPerSecond)*1000000);

    setDirection(dir);

    for(int i=0;i<steps;i++) {
        digitalWrite(stepPin,HIGH);
        delayMicroseconds(2);
        digitalWrite(stepPin,LOW);
        delayMicroseconds(microSecondsPerStep);
    }
}

void StepperDriver::setDirection (bool dir) {
    direction = dir;
    digitalWrite(dirPin,dir);
    delayMicroseconds(1);
}

bool StepperDriver::getDirection() {
    return direction;
}
