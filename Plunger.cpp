#include "Plunger.h"
#include "application.h"

Plunger::Plunger() {

  //initialize plunger state
  plungerParked = false;
}

void Plunger::setup(int stepPin,int dirPin, int enablePin,int parkedPin) {

    this->parkedPin = parkedPin;

    stepper.setup(stepPin,dirPin,enablePin);

    //inputs
    pinMode(parkedPin, INPUT_PULLDOWN);


    //attachInterrupt(parkedPin, [=](){return this->setPlungerParked();}, CHANGE );
    attachInterrupt(parkedPin, [=](){return this->setPlungerParked();}, CHANGE );

    plungerParked = digitalRead(parkedPin);
}

void Plunger::park() {
    /*this will resolve a jam. If any movements are called for, and isParked() is still false, the calling method can call park() to resolve it.
    It will move a little bit in one way, and then move a little the other way. It'll repeat with bigger movement distances until the park interrupt triggers and sets the
    plungerparked state. If park() is called but plungerParked is true already, nothing will happen*/
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
