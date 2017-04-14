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
  if (debug)
    Serial.println("Setting Default PID Values");
  rxValues.P = defaultP;
  rxValues.I = defaultI;
  rxValues.D = defaultD;
}

