SYSTEM_MODE(MANUAL);

#define DEBUGGING true;

#ifndef pins_h
  #define pins_h
  #include "pins.h"
#endif

#include "Device.h"



void setup() {

    #ifdef DEBUGGING
        Serial.begin(9600);
    #endif

    flywheelESC.attach(escPin);  // attaches pin to servo object

    // Initialize pins
    // It's important you do this here, inside the setup() function rather than in the loop function.

    //outputs
    pinMode(IrLEDPin, OUTPUT);

    pinMode(cylinderStepperStepPin, OUTPUT);
    pinMode(plungerStepperStepPin, OUTPUT);
    pinMode(stepperEnablePin, OUTPUT);
    digitalWrite(stepperEnablePin, HIGH); // Turn off steppers (HIGH)
    pinMode(stepperDirPin, OUTPUT);
    digitalWrite(stepperDirPin, LOW);
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

    attachInterrupt(plungerParkedPin, setPlungerParked, CHANGE );
    //attachInterrupt(triggerPin, debugPins, CHANGE );*/
    attachInterrupt(chamberPositionPin, setChamberInPosition, CHANGE );
    /*attachInterrupt(modeSwitch, debugPins, CHANGE );*/

    //connect to wifi if it's enabled
    if(digitalRead(wifiEnablePin)) {
        Particle.connect();
    }

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

//the main loop
void loop() {

    maintainNonInterruptState();

    if(digitalRead(wifiEnablePin)) {
        //process wifi events if it's enabled
        Particle.process();
    }

    //read dart in chamber pin when decisions need to be made

    debug("Pin",(int) chamberInPosition);
    delay(50);

    if(digitalRead(triggerPin)) {
        /* considerations:
        -you need to delay pushing the plunger in until the flywheels are up to speed
        -flywheels take a different time to get to max speed depending on how fast the max speed is
        -you CAN rotate the cylinder and find a dart while waiting for this to happen
        if the mode switch is enabled:
            -spin up the flywheels
            -you can't push the plunger in until: flywheels are up to speed, and a dart is in the chamber, and the chamber location switch is high
            -if a dart is not in the chamber, rotate it until it is
            -if the chamber location switch is not high, rotate until it is
        if the mode switch is disabled:
            -spinup the flywheels
            -fire the chamber
            -rotate to the next chamber
            -you cannot fire the plunger until: the flywheels are up to speed, and the chamber location switch is high. rotate until the chamber location switch is high before firing.
        */
    }
}

/***********
Interrupt triggered state maintenance
***********/

void setPlungerParked() {
    plungerParked = digitalRead(plungerParkedPin);
    delay(1);
}

void setChamberInPosition() {
    chamberInPosition = digitalRead(chamberPositionPin);
    delay(1);
}

/*undecided if this is how I want to trigger an advance event. Because if it's interrupt driven,
holding it down won't cause the event to fire each loop. It would be nicer to
check if the button is held for a number of seconds and make decisions on that. eg.
if the button is held less than 350ms, advance one cylinder. If held longer than that
and then released, rotate to the next dart in chamber and stay there.*/
void fireAdvanceEvent() {
  if(!digitalRead(advanceButtonPin)) {
      advancingCylinder = true;
      delay(1);
  }

}

/************
Change the device state to reflect the pins that can not (or should not) be interrupt driven
This means that we can't have long running motor rotations because we need to
get state mid-rotation for a few things. If the board gets changed so that all
input happens on interrupt capable pins, this could be a lot smarter

Maybe look into driving the LED using a 555 timer, freeing up the LED pin to be
used for the chamberPositionPin (D0) instead. The board wouldn't need to be modified.
*/
void maintainNonInterruptState()
{

    //D0 can't use interrupts since it's already used by the Photon's built in mode switch
    /*chamberInPosition = digitalRead(chamberPositionPin);*/

    dartInChamber = digitalRead(dartInChamberSensorPin);
    //let the readings stabilize (though since the main loop takes longer than this, it's maybe unecessary)
    delay(1);
}

/*************
Debugging and wifi connection
*************/

// Log message to serial
void debug(String message, int value) {
  #ifdef DEBUGGING
    char msg [50];
    //sprintf(msg, message.c_str(), value);
    Serial.println(value);
  #endif
}

//used in setup() to connect to wifi if the wifi pin is enabled
void connect() {
  if (Particle.connected() == false) {
    Particle.connect();
  }
}
