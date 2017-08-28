#include "Cylinder.h"
#include "application.h"

Cylinder::Cylinder() {

    //cylinder speed in microsteps per second
    stepsPerSecond = 2000;//6250;

    //we'll increment the distance each rotation will go by this number of steps while parking
    parkingStepIncrement = 32;

    //initialize chamber state
    cylinderParked = false;

    cylinderCount = 12;

    //maximum numver of chambers to go without hitting a switch before we give up
    maxChamberPositions = 3;
}

void Cylinder::setup(int stepPin,int dirPin, int enablePin,int parkedPin, int stepsPerRotation) {

    this->parkedPin = parkedPin;
    this->stepsPerRotation = stepsPerRotation;

    // in the current configuation, this is 533.33333333 so truncate it down to 533.
    // We'll lose a step every once in a while so to keep it accurate we'll move

    // todo - decide if you want to go move until the switch is hit instead of
    this->stepsPerChamber = (int) (stepsPerRotation / 12);

    //if we haven't found a parking spot and we've tried what should have been 3 chambers, bail

    this->maxParkingSteps = maxChamberPositions * stepsPerChamber;

    stepper.setup(stepPin,dirPin,enablePin);

    //inputs
    pinMode(parkedPin, INPUT_PULLDOWN);
    attachInterrupt(parkedPin, [=](){return this->setCylinderParked();}, CHANGE );
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
Serial.println("Starting advance one chamber");
    stepper.enable();

    Serial.printlnf("Steps per chamber %f", this->stepsPerChamber);
    Serial.printlnf("Moving %f", (float) stepsPerChamber*0.8);

    //get most of the way to the next chamber
    stepper.move(1, (int) (stepsPerChamber*0.8), stepsPerSecond);
    int stepsToRotate = 4;
    int stepsRotated = 0;

    Serial.println("Done moving 0.8");
    Serial.printlnf("Cylinder Parked: %i", cylinderParked);
    //start checking while spinning the last few steps to the switch
    do{
        stepper.move(1, stepsToRotate, stepsPerSecond);
        Serial.printlnf("Moved %i, Cylinder Parked: %i", stepsToRotate, cylinderParked);
        stepsRotated += stepsToRotate;
        //the end condition is the safety fuse. We should normally find the
        //switch before this loop terminates
    } while(stepsRotated <= maxParkingSteps && !cylinderParked);

    Serial.println("Done Rotation");

    //TODO don't use a delay to put the brakes on, set a state where we check if it's time to turn it off next loop cycle, this is adding a delay to the advance button press
    delay(100);
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
    //Serial.printlnf("cylinder is %i",cylinderParked);
}
