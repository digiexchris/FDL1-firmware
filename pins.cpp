//pins.h

// Define the pins we're going to call pinMode on

//input
//interrupt attached pins
int chamberPositionPin = A4; // green //chamber position microswitch under the main body
int plungerParkedPin = DAC; // blue  //microswitch in the rear that is hit when the plunger is all the way

int triggerPin = A3;  //TX;
int dartInChamberSensorPin = D2; //IR Sensor under the main body
int wifiEnablePin = RX; //The jumper to enable/disable wifi
int speedPotPin = A2; //Speed control potentiometer
int advanceButtonPin = D0; // orange // advance the cylinder to the next chamber //NOTE: D0 is not an interrupt capable pin!
int modeSwitch = D1; // brown // the mode switch on the left side

//output
int IrLEDPin = D3; //IR LED under the main body
int cylinderStepperStepPin = D5; //Cylinder stepper step pin
int plungerStepperStepPin = D4; //Plunger stepper step pin
int stepperEnablePin = D6; //enable pin for both drivers
int stepperDirPin = D7; //direction pin for both drivers
int escPin = TX; //esc signal pin (servo spec)
int modeSwitchPower = WKP; //supply the mode switch with power
