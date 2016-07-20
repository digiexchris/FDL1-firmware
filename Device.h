//Create some device wide variables
Servo flywheelESC;  // create servo object to control a ESC

/*double speedValue = 0.0;
unsigned long disableMillis = millis();
int stepperWarmup = 80;
int cylinderPositionAdvanceDelay = 380;
unsigned long flywheelSpinup = 400;*/

//device state
bool dartInChamber = false;
bool chamberInPosition = false;
bool plungerParked = false;
bool advancingCylinder = false;

//need to know if it's firing so that we can keep the cylinder from advancing or doing anything else
//other than completing a firing cycle
bool firing = false;
