//StepperDriver
#ifndef STEPPERDRIVER_H
#define STEPPERDRIVER_H

#include "application.h"

class StepperDriver
{
private:
  int stepPin;
  int dirPin;
  int enablePin;

  bool direction;
  bool enabled;

public:
  StepperDriver(int stepPin, int dirPin, int enablePin);
  void setup();
  void enable();
  void disable();
  void setDirection( int dir );
  bool step(int steps, int stepsPerSecond);
};

#endif
