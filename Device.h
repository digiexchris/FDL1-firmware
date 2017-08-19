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
  Chamber chamber;

  /*double speedValue = 0.0;
  unsigned long disableMillis = millis();
  int stepperWarmup = 80;
  int cylinderPositionAdvanceDelay = 380;
  unsigned long flywheelSpinup = 400;*/

  //device state
  bool dartInChamber;
  volatile bool chamberInPosition;
  bool advancingCylinder;
  int stepsPerRotation;

  //the current millis() time that the button changed state
  //used for checking how much time has passed since the switch was turned on
  //so we can decide if we're going to do a single advance (short press) or
  //advance to next dart (long press)
  int advanceButtonOnTime;
  int advanceButtonOffTime;

  //need to know if it's firing so that we can keep the cylinder from advancing or doing anything else
  //other than completing a firing cycle
  bool firing;


public:
  Device();
  void maintainNonInterruptState();
  bool isReadyToFire();
  void park();
  void setup();
  void advanceOneChamber();
  void advanceToDart();

  //run one tick
  void pulse();
};

#endif
