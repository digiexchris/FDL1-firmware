#include "Device.h"
#include "application.h"

Device::Device() {

  //initialize device state
  dartInChamber = false;
  stepsPerRotation = 6400;

  advanceButton = 0;
  advanceButtonOffTime = 0;
  advanceButtonOnTime = 0;

  //is the trigger held down or not
  triggerButton = 0;

  spinupTime = 0;
}

void Device::setup() {
    flywheelESC.attach(escPin);  // attaches pin to servo object
    plunger.setup(plungerStepperStepPin,stepperDirPin,stepperEnablePin,plungerParkedPin,stepsPerRotation);
    cylinder.setup(cylinderStepperStepPin,stepperDirPin,stepperEnablePin,chamberPositionPin,stepsPerRotation);
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

    //we'll need the trigger button state before setup is done to determine power mode and setup the ESCs
    pinMode(triggerPin, INPUT_PULLDOWN);
    triggerButton = digitalRead(triggerPin);

    pinMode(dartInChamberSensorPin, INPUT_PULLDOWN);
    pinMode(wifiEnablePin, INPUT_PULLDOWN);
    //pinMode(speedPotPin, INPUT);
    pinMode(advanceButtonPin, INPUT_PULLDOWN);
    pinMode(modeSwitch, INPUT_PULLDOWN);

    //attachInterrupt(triggerPin, debugPins, CHANGE );*/

    /*attachInterrupt(modeSwitch, debugPins, CHANGE );*/

    //initialize the esc. Give it full throttle with the trigger held down when the power is turned on, then minimum throttle.
    //if it's turned on with the trigger not held down, just give it minimum throttle to enable the esc.
    //this is in case the esc does not give the motors full rpm. In RC terms, this is calibrating the end stops of the throttle and should only be needed once.

    while(triggerButton == HIGH){
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

    /* since there's limited interupt pin availability I'm sacrificing some state change detection capability.
    In some cases if the machine is busy doing something else we'll miss that a button was pressed. Considering
    this blaster is only really cabable of doing one thing at a time, this isn't a big deal. eg. if someone toggles
    the advance button wile a firing cycle is in progress the state change will be missed, but we don't want
    a cylinder advance to happen while a firing cycle is in progress anyway. Not only is this expected behavior, but
    it's preferred so that we don't have to maintain an event queue for things we don't care about. */
    maintainNonInterruptState();

    //firing where the device is in a fireable state takes priority over all else so it's at the top of the list
    /*if(triggerButton) {


        //TODO make a dart jam detector with the pin freed up by the potentiometer
        //TODO for now, if it wasn't parked when we got here, flip a toggle and don't fire. Unflip that toggle when the mode switch is hit. Turn mode into a reset.
        //TODO turn on esc

        int startFlywheelTime = millis();

        //make sure they're in position. If they are these will take no action.
        plunger.park();
        cylinder.park();

        spinupTime = millis() - startFlywheelTime;

        //if parking took some time we might already be up to speed.
        if(spinupTime < getSpoolUpTime()) {
            delay(getSpoolUpTime() - spinupTime);
            spinupTime = millis() - startFlywheelTime;
        }

        if(dartInChamber) {
            //fire();
        } else {
            if(advanceToDart()) {
                //fire();
            }
        }

        //since this is returning from pulse() and the triggerButton check is first, if we keep holding down the trigger button it will keep firing.
        return;
    }

    if(!triggerButton) {
        //start spinning down the flywheel counter, so if we pull the trigger fast enough we don't need to spin up as fast

        //do something like figure out the spin-down rate based on the current power setting and start reducing the start flywheel time by
        //some ratio between the time it takes to spin down from the current power level and the spinupTime
    }*/

    //if the advance button was triggered in the last 450ms for less than 350ms, advance one chamber
    int currentTime = millis();
    if(advanceButton && advanceButtonOffTime - advanceButtonOnTime < 350 && currentTime - advanceButtonOffTime  < 400) {
        advanceOneChamber();

        //let the loop have an opportunity to detect the next state as fast as possible
        return;
    }

    //if the advance button was released in the last 700ms for greater than 350ms, advance to the next dart
    if(advanceButton && advanceButtonOffTime - advanceButtonOnTime > 350 && currentTime - advanceButtonOffTime < 2000) {

        if(dartInChamber) {
            return;
        } else {
            advanceOneChamber();
            return;
        }
    }

    //TODO do something with the advanceButtonOnTime and OffTime variables, they'll overflow if nothing happens in a long enough time.
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

/**
    integer between 0 and max servo pwm value (180 is the original FDL max esc value)
    TODO when power levels is implemented make this dynamic. Middle of the range for now.
*/
int Device::getPowerLevel() {
    return 90;
}

/**
    the time it takes to spool up the flywheels depends on what the max rpm we're going to
    stop accelerating at. I've used audacity to measure how long it takes at each power level
*/
int Device::getSpoolUpTime() {
    //TODO when you've got power levels implemented make this dynamic based on power level
    return 400;
}

void Device::advanceOneChamber() {
    cylinder.advanceOneChamber();
}

bool Device::advanceToDart() {
    //TODO you can only advance two rotations to find a dart, any more than that, and abort the sequence

    //return true to fire or false on error.
}

void Device::park() {
    plunger.park();
    //cylinder.park();
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

    //I had my switch wired inverted, nbd
    triggerButton = !digitalRead(triggerPin);

    //D0 can't use interrupts since it's already used by the Photon's built in mode switch
    //maintain advance button sate
    int previousAdvanceButton = advanceButton;
    advanceButton = !digitalRead(advanceButtonPin);
    if(previousAdvanceButton != advanceButton && advanceButton == 1) {
        advanceButtonOnTime = millis();
    }
    if(previousAdvanceButton != advanceButton && advanceButton == 0) {
        advanceButtonOffTime = millis();
    }
    //end maintaining advance button state

    dartInChamber = digitalRead(dartInChamberSensorPin);
}
