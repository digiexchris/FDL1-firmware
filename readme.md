FDR
Foam Dart Robot

FDR is a foam dart gun firmware for the Particle Photon. It is meant as a replacement for the firmware that controls the excellent FDL-1 by Jessie Kovarovics. 

The goals of FDR for the FDL-1 is to

- decrease delays in the firing cycle
- reduce the operator workload a bit, so that angry coworkers can just pick it up, turn it on, and shoot you with as little explanation as possible
- make the settings default into a safe mode, so that when that coworker shoots you in the face it's not running at 'welt-inducing' and instead is set at 'merely terrifying'
- provide some self-checks and calibrations so when people stick their finger in that slot in the back and bump the plunger off the park position, it can fix that. It'll also make sure it's safe as possible to move, and attempt to guess if there's a jam.
- Be easy to modify for your needs or other similar blaster designs.
- Save power a bit by turning the steppers off faster

Features
- click the advance switch to move the cylinder one chamber
- hold the advance switch for more than about a third of a second and release to advance to the next chamber that has a dart in it.
- default power on power is 1/3 speed. To set 2/3 speed, hold the advance switch while turning on. To set full eyeball crushing power, hold down the advance switch, the reset button, and turn it on. Yes, you'll need 3 hands on purpose.
- pull the trigger to fire.

Usage
FDR requires swapping some wires around on the Photon so that some of the sensors can use interrupt capable pins in order to chop a few nanoseconds off the firing cycle. See the wiring guide. You'll also need to claim your Photon, attach it to your network, and somehow get this code compiled and installed. I recommend particle-dev, Particle's atom based dev environment. Then it should Just Work (insert works on my machine joke here).