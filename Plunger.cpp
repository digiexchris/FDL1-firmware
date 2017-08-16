#include "Plunger.h"
#include "application.h"

Plunger::Plunger() {

    stepsPerSecond = 2000;
    //we'll increment the distance each rotation will go by this number of steps while parking
    parkingStepIncrement = 10;
    //initialize plunger state
    plungerParked = false;
}

void Plunger::setup(int stepPin,int dirPin, int enablePin,int parkedPin, int stepsPerRotation = 6400) {

    this->parkedPin = parkedPin;
    this->stepsPerRotation = stepsPerRotation;

    stepper.setup(stepPin,dirPin,enablePin);

    //inputs
    pinMode(parkedPin, INPUT_PULLDOWN);
    attachInterrupt(parkedPin, [=](){return this->setPlungerParked();}, CHANGE );
    plungerParked = digitalRead(parkedPin);
}


/*park() will attempt to resolve a partially deployed plunger (like if someone sticks their finger in the slot in the back or a jam has occurred).

If any movements are called for, and plungerParked is still false, the calling method can call park() to resolve it.
It will move one rotation in one direction, and then move one rotation the other way.
If park() is called but plungerParked is true already, nothing will happen
NOTE: this method blocks until it's complete!*/

void Plunger::park() {

    //if it's already parked don't do anything
    if(plungerParked) {
        return;
    }

    //try first in one direction
    if(Plunger::parkingMove(1)) {
        return;
    }

    //then try in the other direction
    if(Plunger::parkingMove(0)) {
        return;
    }
}

bool Plunger::parkingMove(int dir) {
    int stepsRotated = 0;
    int stepIncreaseSize = 10;

    stepper.enable();

    do {
        stepsRotated += stepIncreaseSize;

        //try to find the switch in one direction
        stepper.move(1,stepsRotated,stepsPerSecond);
        if(plungerParked) {
            return true;
        }

    } while(stepsRotated <= stepsPerRotation);

    stepper.disable();
    return false;
}

bool Plunger::isParked() {
    return plungerParked;
}

void Plunger::fire() {
    if(!plungerParked) {
        park();
    }

    stepper.move(1, 100, 60);
}

/***********
Interrupt triggered state maintenance
***********/
void Plunger::setPlungerParked() {
    plungerParked = digitalRead(parkedPin);
}
