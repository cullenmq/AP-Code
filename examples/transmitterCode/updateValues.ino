/* Main function for Serial
   takes in commands from the Serial port
   need a '*' character leading your command
   possible commands:
     k              : kills the quadcopter (kill switch)
     t 1.5 1.1 .9   : updates PID to P=1.5, I=1.1, D=.9
     c 20           : updates channel to 20
*/
void updateSerial() {
  char cmd = ' ';
  do
  {
    if (!Serial.available())
      return;
  }
  while ((cmd = Serial.read()) != '*');
  cmd = Serial.read();
  switch (cmd) {
    case 't':
      P = Serial.parseFloat();
      I = Serial.parseFloat();
      D = Serial.parseFloat();
      if (debug) {
        Serial.print("P= ");
        Serial.println(P);
        Serial.print("I= ");
        Serial.println(I);
        Serial.print("D= ");
        Serial.println(D);
      }
      break;
    case 'c':
      channel = Serial.parseInt();
      controller.setChannel(channel);
      Serial.print("Channel is: ");
      Serial.println(channel);
      break;
    case 'k':
      rxValues.safety = 1;
      Serial.println("KILLING THE QUAD!!!");
      break;
    case 'r':
      Serial.println("calibration!");
      calibrateJoysticks();
      break;

  }
  Serial.println(cmd);
}
void calibrateJoysticks()
{
  throttleCal = 0 - (analogRead(throttlePin) >> 2);
  rollCal = 128 - (255 - (analogRead(rollPin) >> 2));
  pitchCal = 128 - (analogRead(pitchPin) >> 2);
}
uint8_t checkBounds(int16_t check) {
  if (check > 255)
    return 255;
  if (check < 0)
    return 0;
  return check;
}
void updateJoysticks() {
  //store data into 1 byte chuncks
  int16_t tempThrottle = (analogRead(throttlePin) >> 2) + throttleCal;
  int16_t tempRoll = (255 - (analogRead(rollPin) >> 2)) + rollCal + rollTrim.getValue();
  int16_t tempPitch = (analogRead(pitchPin) >> 2) + pitchCal + pitchTrim.getValue();

  rxValues.throttle = checkBounds(tempThrottle);
  rxValues.roll = checkBounds(tempRoll);
  rxValues.pitch = checkBounds(tempPitch);
  rxValues.yaw = 255 - (analogRead(yawPin) >> 2) + yawTrim.getValue();
  rxValues.flip = flipButton.checkPin();
  rxValues.highspeed = modeButton.checkPin();
}
void updateTrims() {
  pitchTrim.updateValue();
  yawTrim.updateValue();
  rollTrim.updateValue();
  rxValues.P = P;
  rxValues.I = I;
  rxValues.D = D;
}
void auxIndicator(bool isLED) {
  digitalWrite(auxLEDPin, isLED);
  //if (debug && isLED)
  //Serial.println("AUX LED ON!");
}
void updateBattery() {
  int batRead = analogRead(contBattPin);
  //calc bat voltage (mV)
  unsigned long batVolt = (batRead * logicVolt * (R3 + R4)) / (R4) * 1000 / 1023;
  if (batVolt < 3700) {
    //Serial.println("Controller Battery Low!!!");
    digitalWrite(contBattLED, HIGH);
  }
}
