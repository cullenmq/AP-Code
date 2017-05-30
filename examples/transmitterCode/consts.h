#ifndef CONST_H
#define CONST_H
/*********************
 **** PIN DEFINES ****
 *********************/
#define upYawPin A4
#define downYawPin A3
#define upPitchPin  7
#define downPitchPin 6
#define upRollPin 5
#define downRollPin 4
#define throttlePin A1
#define yawPin A2
#define rollPin A7
#define pitchPin A6
#define flipPin A0
#define modePin 8
#define CEpin 10
#define CSpin 9

// LED Pins
#define contBattLED 2
#define auxLEDPin 3
// analog pin for controller battery reading
#define contBattPin A5

#define KILL_DURATION 1000
/********************************
 **** Voltage Divider Consts ****
 ********************************/
#define R4 3300
#define R3 1000
#define logicVolt 3.3


#endif
