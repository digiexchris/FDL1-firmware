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
}

void StepperDriver::disable() {
    enabled = false;
    digitalWrite(enablePin, HIGH);
}
void StepperDriver::move(bool dir, int steps, int stepsPerSecond = 1000) {
    unsigned long millisecondsPerStep = (1/(float)stepsPerSecond)*1000;

    setDirection(dir);

    for(int i=0;i<steps;i++) {
        digitalWrite(stepPin,HIGH);
        delayMicroseconds(2);
        digitalWrite(stepPin,LOW);
        delay(millisecondsPerStep);
    }
}

void StepperDriver::setDirection (bool dir) {
    direction = dir;
    digitalWrite(dirPin,dir);
}

bool StepperDriver::getDirection() {
    return direction;
}
