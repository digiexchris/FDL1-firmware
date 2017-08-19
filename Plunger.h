#ifndef PLUNGER_H
#define PLUNGER_H

#include "application.h"
#include "StepperDriver.h"

class Plunger
{
private:
  StepperDriver stepper;  // create servo object to control a stepper
  //plunger state
  volatile bool plungerParked;
  int parkedPin;
  int stepsPerSecond;
  int stepsPerRotation;
  int parkingStepIncrement;

  bool parkingMove(int direction);
  void setPlungerParked();


public:
  Plunger();
  bool isParked();
  void park();
  void fire();
  void setup(int stepPin, int dirPin, int enablePin, int parkedPin, int stepsPerRotation);
};

#endif
