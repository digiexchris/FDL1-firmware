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
    digitalWrite(enablePin,HIGH);
}
void StepperDriver::move(int dir, int steps, int stepsPerSecond) {
    
}

void StepperDriver::setDirection (int dir) {
    direction = dir;
    digitalWrite(dirPin,dir);
}
