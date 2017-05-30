void initChannel() {
  Serial.println("Initializing Default Channel");

  // read in EEPROM
  channel = EEPROM.readByte(ChannelAddr);
  // if data has been written, update the values
  if ((channel == 0xFF))
  {
    Serial.println("first time writing to Channel EEPROM");
    channel = 30;
    EEPROM.writeBlock(ChannelAddr, &channel);
  }
  Serial.print("Channel is: ");
  Serial.println(channel);
}
void initButtons() {
  if (debug)
    Serial.println("intializing trim buttons");
  pitchTrim.init();
  rollTrim.init();
  yawTrim.init();
  //joysticks
  flipButton.init();
  modeButton.init();
  pinMode(pitchPin, INPUT);
  pinMode(rollPin, INPUT);
  pinMode(yawPin, INPUT);
  pinMode(throttlePin, INPUT);
  // define leds
  pinMode(contBattLED, OUTPUT);
  pinMode(auxLEDPin, OUTPUT);
  // controller bat input
  pinMode(contBattPin, INPUT);
}
void initPID() {
  PIDVal test;
  test.P = -1;
  test.I = -1;
  test.D = -1;
  while (!EEPROM.isReady()) {};
  Serial.println("Initializing Default PID Values");

  // read in EEPROM
  EEPROM.readBlock(PIDAddr, mPID);
  Serial.print("PID Address: ");
  Serial.println(PIDAddr);
  Serial.print("\t"); Serial.print(mPID.P);
  Serial.print("\t"); Serial.print(mPID.I);
  Serial.print("\t"); Serial.println(mPID.D);
  // if data has been written, update the values
  if (isnan(mPID.P) || isnan(mPID.I) || isnan(mPID.D))
  {
    Serial.println("first time writing to PID EEPROM");
    mPID.P = defaultP;
    mPID.I = defaultI;
    mPID.D = defaultD;
    EEPROM.updateBlock(PIDAddr, mPID);
  }
}
void initEEPROMAddrs() {
  PIDAddr = EEPROM.getAddress(sizeof(PIDVal));
  CalAddr = EEPROM.getAddress(sizeof(CalVal));
  ChannelAddr = EEPROM.getAddress(sizeof(uint8_t));
}
void initCal() {
  Serial.println("Initializing Default Calibration Values");

  // read in EEPROM
  EEPROM.readBlock(CalAddr, mCal);
  // if data has been written, update the values
  if ((mCal.roll == 0xFF) || (mCal.pitch == 0xFF) || isnan(mCal.throttle == 0xFF))
  {
    Serial.println("first time writing to Calibration EEPROM");
    mCal.roll = 0;
    mCal.pitch = 0;
    mCal.throttle = 0;
    EEPROM.writeBlock(CalAddr, mCal);
  }
}
