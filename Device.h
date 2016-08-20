#ifndef DEVICE_H
#define DEVICE_H

#include "application.h"
#include "pins.h"
#include "Plunger.h"

class Device
{
private:
  //Create some device wide variables
  Servo flywheelESC;  // create servo object to control a ESC
  Plunger plunger;
  /*double speedValue = 0.0;
  unsigned long disableMillis = millis();
  int stepperWarmup = 80;
  int cylinderPositionAdvanceDelay = 380;
  unsigned long flywheelSpinup = 400;*/

  //device state
  bool dartInChamber;
  bool chamberInPosition;
  bool plungerParked;
  bool advancingCylinder;

  //need to know if it's firing so that we can keep the cylinder from advancing or doing anything else
  //other than completing a firing cycle
  bool firing;


public:
  Device();
  void maintainNonInterruptState();
  void setPlungerParked();
  void setChamberInPosition();
  bool isReadyToFire();
  void setup();
};

#endif
