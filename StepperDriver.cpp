#include "StepperDriver.h"

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
    delayMicroseconds(2);
}

void StepperDriver::disable() {
    enabled = false;
    digitalWrite(enablePin, HIGH);
    delayMicroseconds(2);
}
void StepperDriver::move(bool dir, int steps, int stepsPerSecond = 1000) {
    unsigned long microSecondsPerStep = (1/(float)stepsPerSecond)*1000000;

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
