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
      mPID.P = Serial.parseFloat();
      mPID.I = Serial.parseFloat();
      mPID.D = Serial.parseFloat();
      if (debug) {
        Serial.print("P= ");
        Serial.println(mPID.P);
        Serial.print("I= ");
        Serial.println(mPID.I);
        Serial.print("D= ");
        Serial.println(mPID.D);
      }
      break;
    case 'p':
      Serial.print(" :\t"); Serial.print(rxValues.throttle);
      Serial.print("\t"); Serial.print(rxValues.yaw);
      Serial.print("\t"); Serial.print(rxValues.pitch);
      Serial.print("\t"); Serial.print(rxValues.roll);
      Serial.print("\t"); Serial.print(rxValues.flip);
      Serial.print("\t"); Serial.print(rxValues.highspeed);
      Serial.print("\t"); Serial.print(rxValues.P);
      Serial.print("\t"); Serial.print(rxValues.I);
      Serial.print("\t"); Serial.println(rxValues.D);
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
      calibrateJoysticks();
      break;
    case 'e':
      char cmdEEPROM = Serial.read();
      if (cmdEEPROM == 't')
        savePID();
      else if (cmdEEPROM == 'r')
        saveCal();
      break;
  }
  Serial.println(cmd);
  Serial.flush();
}
void calibrateJoysticks()
{
  Serial.println("calibration!");
  mCal.throttle = 0 - (analogRead(throttlePin) >> 2);
  mCal.roll = 128 - (255 - (analogRead(rollPin) >> 2));
  mCal.pitch = 128 - (analogRead(pitchPin) >> 2);
  Serial.print(mCal.throttle); Serial.print('\t');
  Serial.print(mCal.pitch); Serial.print('\t');
  Serial.println(mCal.roll);
}
void saveCal()
{
  Serial.println("Saving Cal Data to EEPROM!");
  EEPROM.updateBlock(CalAddr, mCal);
}
void savePID()
{
  Serial.println("Saving PID Data to EEPROM!");
  Serial.print("PID Address: ");
  Serial.println(PIDAddr);
  if (!EEPROM.writeBlock(PIDAddr, mPID))
    Serial.println("PID EEPROM update failed!");
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
  int16_t tempThrottle = (analogRead(throttlePin) >> 2) + mCal.throttle;
  int16_t tempRoll = (255 - (analogRead(rollPin) >> 2)) + mCal.roll + rollTrim.getValue();
  int16_t tempPitch = (analogRead(pitchPin) >> 2) + mCal.pitch + pitchTrim.getValue();

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
  rxValues.P = mPID.P;
  rxValues.I = mPID.I;
  rxValues.D = mPID.D;
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
