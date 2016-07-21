SYSTEM_MODE(MANUAL);

#define DEBUGGING true;

#ifndef pins_h
  #define pins_h
  #include "pins.h"
#endif

#include "Device.h"

Device device;

void setup() {

    #ifdef DEBUGGING
        Serial.begin(9600);
    #endif

    device.setup();

    //connect to wifi if it's enabled
    if(digitalRead(wifiEnablePin)) {
        Particle.connect();
    }


}

//the main loop
void loop() {

    device.maintainNonInterruptState();

    if(digitalRead(wifiEnablePin)) {
        //process wifi events if it's enabled
        Particle.process();
    }

    //read dart in chamber pin when decisions need to be made

    debug("Pin",(int) device.getChamberInPosition());
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

/*undecided if this is how I want to trigger an advance event. Because if it's interrupt driven,
holding it down won't cause the event to fire each loop. It would be nicer to
check if the button is held for a number of seconds and make decisions on that. eg.
if the button is held less than 350ms, advance one cylinder. If held longer than that
and then released, rotate to the next dart in chamber and stay there.*/
/*void fireAdvanceEvent() {
  if(!digitalRead(advanceButtonPin)) {
      advancingCylinder = true;
      delay(1);
  }

}*/



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
