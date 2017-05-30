#include <EEPROMex.h>
#include <EEPROMVar.h>
/*
  Name:		transmitterCode.ino
  Created:	1/5/2017 9:21:45 AM
  Author:	Cullen Quine
*/
// INCLUDE STATEMENTS
#include "Controller.h"
#include "trim.h"
#include "buttonManager.h"
#include "printf.h"
#include "consts.h"

// USER DEFINED CONSTANTS
// toggle Serial input/output
#define debug 1
// default channel number
uint8_t channel = 30;
// address of EEPROM
int address = 0;
// PID default
#define defaultP 1
#define defaultI 0
#define defaultD .5

int PIDAddr, CalAddr, ChannelAddr;
struct PIDVal {
  float P = defaultP, I = defaultI, D = defaultD;
};
PIDVal mPID;
struct CalVal {
  int8_t roll, pitch, throttle;
};
CalVal mCal;

/************************
 *** GLOBAL VARIABLES****
 ************************/
// radio
RF24 radio(CEpin, CSpin);
// package struct
rx_values_t rxValues;
// timing variables for timing loop
unsigned long killTime = 0;
bool wasKillLow = true, quadKill = false;

// trim variables for adjusting for drift
trim rollTrim(upRollPin, downRollPin);
trim pitchTrim(upPitchPin, downPitchPin);
trim yawTrim(upYawPin, downYawPin);
int8_t throttleCal = 0, pitchCal = 0, rollCal = 0;
// controller variable: in charge of sending/receiving true means this is for the controller (swaps addresses)
Controller controller(&radio, channel, true);
// two buttons on the joysticks for whatever you want
buttonManager flipButton(flipPin, 0x0, true), modeButton(modePin, 0x0, true);
/************************
 *****SETUP FUNCTION*****
 ************************/
//SETUP FUNCTION: Initializes Serial, controller, buttons, and radio
void setup() {
  //initializes the radio

  if (debug) {
    Serial.begin(38400);
    Serial.setTimeout(10);
    printf_begin();
  }
  initEEPROMAddrs();
  initChannel();
  controller.init();
  controller.setChannel(channel);
  //buttons
  //sets pinModes for buttons/LEDs
  initButtons();
  //sets up the default PID values
  initPID();
  //sets up calibration offsets
  initCal();
  if (debug) {
    Serial.println("printing radio details");
    radio.printDetails();
  }
  killTime = millis();
}
//LOOP: Main function of the Controller
void loop() {
  if (debug)
    // check Serial for user input
    updateSerial();
  // update the current battery voltage of the controller
  updateBattery();
  // read in the values from the joysticks
  updateJoysticks();
  // check to see if a trim button was pressed and update the trim value
  updateTrims();
  // send the struct to the quadcopter

  controller.send(&rxValues);
  // receive a struct from the quadcopter (if available)
  if (controller.receive(&rxValues)) {
    // update the quadcopter light
    auxIndicator(rxValues.auxLED);
  }
}

