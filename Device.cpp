#include "Device.h"
#include "application.h"

Device::Device() {

  //initialize device state
  dartInChamber = false;
  chamberInPosition = false;
  plungerParked = false;
  advancingCylinder = false;

  //need to know if it's firing so that we can keep the cylinder from advancing or doing anything else
  //other than completing a firing cycle
  bool firing = false;
}

void Device::setup() {
    flywheelESC.attach(escPin);  // attaches pin to servo object

    // Initialize pins
    // It's important you do this here, inside the setup() function rather than in the loop function.

    //outputs
    pinMode(IrLEDPin, OUTPUT);

/**replace this with the Plunger object setup, which will then setup the Stepper, which will then set the pinmode and state
    pinMode(cylinderStepperStepPin, OUTPUT);
    pinMode(plungerStepperStepPin, OUTPUT);
    pinMode(stepperEnablePin, OUTPUT);
    digitalWrite(stepperEnablePin, HIGH); // Turn off steppers (HIGH)
    pinMode(stepperDirPin, OUTPUT);
    digitalWrite(stepperDirPin, LOW);*/
    pinMode(modeSwitchPower, OUTPUT);
    digitalWrite(modeSwitchPower, HIGH);
    /*pinMode(escPin, LOW);*/ // I don't think escPin requires initialization since Stepper does it

    //inputs
    pinMode(chamberPositionPin, INPUT_PULLDOWN);
    pinMode(plungerParkedPin, INPUT_PULLDOWN);
    pinMode(triggerPin, INPUT_PULLDOWN);
    pinMode(dartInChamberSensorPin, INPUT_PULLDOWN);
    pinMode(wifiEnablePin, INPUT_PULLDOWN);
    pinMode(speedPotPin, INPUT);
    pinMode(advanceButtonPin, INPUT_PULLDOWN);
    pinMode(modeSwitch, INPUT_PULLDOWN);


    //TODO move this into the Plunger object, with the other pin detect considerations
    attachInterrupt(plungerParkedPin, [=](){return this->setPlungerParked();}, CHANGE );
    //attachInterrupt(triggerPin, debugPins, CHANGE );*/

    //TODO move this into the Chamber object
    attachInterrupt(chamberPositionPin, [=](){return this->setChamberInPosition();}, CHANGE );
    /*attachInterrupt(modeSwitch, debugPins, CHANGE );*/

    //initialize the esc. Give it full throttle with the trigger held down when the power is turned on, then minimum throttle.
    //if it's turned on with the trigger not held down, just give it minimum throttle to enable the esc.
    //this is in case the esc does not give the motors full rpm. In RC terms, this is calibrating the end stops of the throttle and should only be needed once.

    while(digitalRead(triggerPin) == HIGH){
        flywheelESC.write(180);
    }

    flywheelESC.write(0);
    delay(500);



    //Start PWM on the IR LED
    analogWrite(IrLEDPin,250,38000);
    //determine the initial alignment of the starting chamber, plunger, etc
    chamberInPosition = digitalRead(chamberPositionPin);
    plungerParked = digitalRead(plungerParkedPin);
}

bool Device::getChamberInPosition() {
    return (bool) chamberInPosition;
}

/***********
Interrupt triggered state maintenance
***********/

void Device::setPlungerParked() {
    plungerParked = digitalRead(plungerParkedPin);
    delay(1);
}

void Device::setChamberInPosition() {
    chamberInPosition = digitalRead(chamberPositionPin);
    delay(1);
}

/************
Change the device state to reflect the pins that can not (or should not) be interrupt driven
This means that we can't have long running motor rotations because we need to
get state mid-rotation for a few things. If the board gets changed so that all
input happens on interrupt capable pins, this could be a lot smarter

Maybe look into driving the LED using a 555 timer, freeing up the LED pin to be
used for the chamberPositionPin (D0) instead. The board wouldn't need to be modified.
*/
void Device::maintainNonInterruptState()
{

    //D0 can't use interrupts since it's already used by the Photon's built in mode switch
    /*chamberInPosition = digitalRead(chamberPositionPin);*/

    dartInChamber = digitalRead(dartInChamberSensorPin);
    //let the readings stabilize (though since the main loop takes longer than this, it's maybe unecessary)
    delay(1);
}
