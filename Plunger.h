#ifndef PLUNGER_H
#define PLUNGER_H

#include "application.h"
#include "StepperDriver.h"

class Plunger
{
private:
  StepperDriver stepper;  // create servo object to control a stepper
  //plunger state
  bool plungerParked;
  int parkedPin;

public:
  Plunger();

  bool isParked();
  void park();
  void fire();
  void setPlungerParked();
  void setup(int stepPin, int dirPin, int enablePin, int parkedPin);
};

#endif