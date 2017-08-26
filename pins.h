//pins.h

 #include "application.h"

// Define the pins we're going to call pinMode on

#ifndef PINS_DEF
#define PINS_DEF
  //input
  //interrupt attached pins
  #define chamberPositionPin A4 // green //chamber position microswitch under the main body
  #define plungerParkedPin DAC // blue  //microswitch in the rear that is hit when the plunger is all the way

  #define triggerPin A3  //TX;
  #define dartInChamberSensorPin D2 //IR Sensor under the main body
  #define wifiEnablePin RX //The jumper to enable/disable wifi
  //#define speedPotPin A2 //Speed control potentiometer, not used in the FDR. Used as an LED below.
  #define advanceButtonPin D0 // orange // advance the cylinder to the next chamber //NOTE: D0 is not an interrupt capable pin!
  #define modeSwitch D1 // brown // the mode switch on the left side

  //output
  #define IrLEDPin D3 //IR LED under the main body
  #define cylinderStepperStepPin D5 //Cylinder stepper step pin
  #define plungerStepperStepPin D4 //Plunger stepper step pin
  #define stepperEnablePin D6 //enable pin for both drivers
  #define stepperDirPin D7 //direction pin for both drivers
  #define escPin TX //esc signal pin (servo spec)
  #define modeSwitchPower WKP //supply the mode switch with power
  #define readyToFireLED A2
#endif
