#include "Cylinder.h"
#include "application.h"

Cylinder::Chamber() {

    //plunger speed in microsteps per second
    stepsPerSecond = 6250;

    //we'll increment the distance each rotation will go by this number of steps while parking
    parkingStepIncrement = 32;

    //initialize chamber state
    cylinderParked = false;

    cylinderCount = 12;

    // in the current configuation, this is 533.33333333 so truncate it down to 533.
    // We'll lose a step every once in a while so to keep it accurate we'll move

    // todo - decide if you want to go move until the switch is hit instead of
    stepsPerChamber = (int) stepsPerRotation / 12

    //if we haven't found a parking spot and we've tried what should have been 3 chambers, bail
    maxChamberPositions = 3
    maxParkingSteps = maxChamberPositions * stepsPerChamber
}

void Cylinder::setup(int stepPin,int dirPin, int enablePin,int parkedPin, int stepsPerRotation = 6400) {

    this->parkedPin = parkedPin;
    this->stepsPerRotation = stepsPerRotation;

    stepper.setup(stepPin,dirPin,enablePin);

    //inputs
    pinMode(parkedPin, INPUT_PULLDOWN);
    attachInterrupt(parkedPin, [=](){return this->setChamberParked();}, CHANGE );
    cylinderParked = digitalRead(parkedPin);
}

void Cylinder::park() {

    //don't do anything if it's already parked
    if(cylinderParked) {
        return;
    }

    stepper.enable();

    //move at least a 1/4 step, faux-acceleration
    int stepsToRotate = 8;

    do {
        //try to find the switch in one direction
        stepper.move(1,stepsToRotate,this->stepsPerSecond);
        if(cylinderParked) {
            stepper.disable();
            return;
        }

        //reverse it and try to find the switch in the other direction
        stepper.move(0,stepsToRotate*2,this->stepsPerSecond);
        if(cylinderParked) {
            stepper.disable();
            return;
        }

        //didn't find it, so go back to the starting point and try again
        stepper.move(1,stepsToRotate,this->stepsPerSecond);
        if(cylinderParked) {
            stepper.disable();
            return;
        }

        stepsToRotate = stepsToRotate + this->parkingStepIncrement;
    } while(stepsToRotate <= stepsPerRotation || stepsToRotate <= maxParkingSteps);

    //hit the max retry limit! bail. It's jammed or the switch is broken or something
    stepper.disable();
    return;
}

void Cylinder::advanceOneChamber() {

    stepper.enable();

    //get most of the way to the next chamber
    stepper.move(1, (int) stepsPerChamber*0.8, stepsPerSecond);
    stepsToRotate = 16;

    //start checking while spinning the last few steps to the switch
    do{
        stepper.move(1, stepsToRotate, stepsPerSecond);

        //the end condition is the safety fuse. We should normally find the
        //switch before this loop terminates
    } while(stepsToRotate <= maxParkingSteps || !cylinderParked);

    stepper.disable();
    return;
}

bool Cylinder::isParked() {
    return cylinderParked;
}

/***********
Interrupt triggered state maintenance
***********/
void Cylinder::setCylinderParked() {
    cylinderParked = digitalRead(parkedPin);
}
