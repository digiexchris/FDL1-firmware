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
  int stepsPerSecond;
  int stepsPerRotation;
  int parkingStepIncrement;

public:
  Plunger();

  bool isParked();
  void park();
  bool parkingMove(int direction);
  void fire();
  void setPlungerParked();
  void setup(int stepPin, int dirPin, int enablePin, int parkedPin, int stepsPerRotation);
};

#endif
