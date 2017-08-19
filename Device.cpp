#include "Device.h"
#include "application.h"

Device::Device() {

  //initialize device state
  dartInChamber = false;
  stepsPerRotation = 6400;

  advanceButtonStateChangeTime = 0;

  //need to know if it's firing so that we can keep the cylinder from advancing or doing anything else
  //other than completing a firing cycle
  firing = false;
}

void Device::setup() {
    flywheelESC.attach(escPin);  // attaches pin to servo object
    plunger.setup(plungerStepperStepPin,stepperDirPin,stepperEnablePin,plungerParkedPin,stepsPerRotation);
    chamber.setup(cylinderStepperStepPin,stepperDirPin,stepperEnablePin,cylinderPositionPin,stepsPerRotation);
    // Initialize pins
    // It's important you do this here, inside the setup() function rather than in the loop function.

    //outputs
    pinMode(IrLEDPin, OUTPUT);
    pinMode(readyToFireLED, OUTPUT);
    digitalWrite(readyToFireLED,LOW);

    //voltage source for the mode switch. Probably should take this from the power rail instead and free
    //up a pin
    pinMode(modeSwitchPower, OUTPUT);
    digitalWrite(modeSwitchPower, HIGH);

    //inputs
    pinMode(chamberPositionPin, INPUT_PULLDOWN);
    pinMode(triggerPin, INPUT_PULLDOWN);
    pinMode(dartInChamberSensorPin, INPUT_PULLDOWN);
    pinMode(wifiEnablePin, INPUT_PULLDOWN);
    pinMode(speedPotPin, INPUT);
    pinMode(advanceButtonPin, INPUT_PULLDOWN);
    pinMode(modeSwitch, INPUT_PULLDOWN);

    //attachInterrupt(triggerPin, debugPins, CHANGE );*/

    /*attachInterrupt(modeSwitch, debugPins, CHANGE );*/

    //initialize the esc. Give it full throttle with the trigger held down when the power is turned on, then minimum throttle.
    //if it's turned on with the trigger not held down, just give it minimum throttle to enable the esc.
    //this is in case the esc does not give the motors full rpm. In RC terms, this is calibrating the end stops of the throttle and should only be needed once.

    while(digitalRead(triggerPin) == HIGH){
        flywheelESC.write(180);
        delay(500);
        flywheelESC.write(0);
        delay(10);
    }

    //Start PWM on the IR LED
    analogWrite(IrLEDPin,250,38000);
    delay(10);
}

//run one tick, the main device logic
void Device::pulse() {
    maintainNonInterruptState();

    //if the advance button was triggered in the last 450ms for less than 350ms, advance one chamber
    int currentTime = millis();
    if(advanceButton == 0 && advanceButtonOffTime - advanceButtonOnTime < 350 && advanceButtonOffTime - currentTime < 400ms) {
        advanceOneChamber()

        //let the loop have an opportunity to detect the next state as fast as possible
        return;
    }

    //if the advance button was released in the last 400ms for greater than 350ms, advance to the next dart
    if(advanceButton == 0 && advanceButtonOffTime - advanceButtonOnTime > 350 && advanceButtonOffTime - currentTime < 400ms) {

        if(dartInChamber) {
            return;
        } else {
            advanceOneChamber()
            return;
        }
    }
}

bool Device::isReadyToFire() {
    if(chamberInPosition
        && dartInChamber
        && plunger.isParked()
        && cylinder.isParked()) {
        digitalWrite(readyToFireLED,HIGH);
        delay(50);
        return true;
    }
    else {
        digitalWrite(readyToFireLED,LOW);
        delay(50);
        return false;
    }
}

void Device::advanceOneChamber() {
    Cylinder.advanceOneChamber();
}

void Device::advanceToDart() {

}

void Device::park() {
    plunger.park();
    cylinder.park();
}

/***********
Interrupt triggered state maintenance
***********/


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
    //maintain advance button sate
    previousAdvanceButton = advanceButton;
    advanceButton = digitalRead(advanceButtonPin);
    if(previousAdvanceButton != advanceButton && advanceButton == 1) {
        advanceButtonOnTime = millis();
    }
    if(previousAdvanceButton != advanceButton && advanceButton == 0) {
        advanceButtonOffTime = millis();
    }
    //end maintaining advance button state

    dartInChamber = digitalRead(dartInChamberSensorPin);
}
