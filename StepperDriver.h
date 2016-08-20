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
  StepperDriver();
  void setup(int stepPin, int dirPin, int enablePin);
  void enable();
  void disable();
  void setDirection( bool dir );
  bool getDirection();
  void move(bool dir, int steps, int stepsPerSecond);
  bool step(int steps, int stepsPerSecond);
};

#endif
