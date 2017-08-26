#ifndef CHAMBER_H
#define CHAMBER_H

#include "application.h"
#include "StepperDriver.h"

class Cylinder
{
private:
  StepperDriver stepper;  // create servo object to control a stepper
  //plunger state
  volatile bool cylinderParked;
  int parkedPin;
  int stepsPerSecond;
  int stepsPerRotation;
  int parkingStepIncrement;
  int cylinderCount;
  int stepsPerChamber;
  int maxParkingSteps;
  int maxChamberPositions;

  bool parkingMove(int direction);
  void setCylinderParked();


public:
  Cylinder();
  void advanceOneChamber();
  bool isParked();
  void park();
  void fire();
  void setup(int stepPin, int dirPin, int enablePin, int parkedPin, int stepsPerRotation);
};

#endif
