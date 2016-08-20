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


/*park() will attempt to resolve a partially deployed plunger (like if someone sticks their finger in the slot in the back).

If any movements are called for, and plungerParked is still false, the calling method can call park() to resolve it.
It will move a little bit in one way, and then move a little the other way. It'll repeat with bigger movement distances
until the park interrupt triggers and sets the plungerparked state.
If park() is called but plungerParked is true already, nothing will happen
NOTE: this method blocks until it's complete!*/

void Plunger::park() {

    stepper.enable();

    int stepsToRotate = 10;

    do {
        //don't do anything if it's already parked
        if(plungerParked) { return; }

        //try to find the switch in one direction
        stepper.move(1,stepsToRotate,stepsPerSecond);
        if(plungerParked) { return; }

        //reverse it and try to find the switch in the other direction
        stepper.move(0,stepsToRotate*2,stepsPerSecond);
        if(plungerParked) { return; }

        //didn't find it, so go back to the starting point and try again
        stepper.move(1,stepsToRotate,stepsPerSecond);
        if(plungerParked) { return; }

        stepsToRotate = stepsToRotate + parkingStepIncrement;
    } while(stepsToRotate <= stepsPerRotation);

    stepper.disable();
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
    delay(1);
}
