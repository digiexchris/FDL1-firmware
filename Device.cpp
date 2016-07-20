#ifndef pins_h
  #define pins_h
  #include "pins.h"
#endif

#include "Device.h"


Device::Device() {

  //initialize device state
  dartInChamber = false;
  chamberInPosition = false;
  plungerParked = false;
  advancingCylinder = false;

  //need to know if it's firing so that we can keep the cylinder from advancing or doing anything else
  //other than completing a firing cycle
  bool firing = false;
}

/***********
Interrupt triggered state maintenance
***********/

void Device::setPlungerParked() {
    plungerParked = digitalRead(plungerParkedPin);
    delay(1);
}

void Device::setChamberInPosition() {
    chamberInPosition = digitalRead(chamberPositionPin);
    delay(1);
}
