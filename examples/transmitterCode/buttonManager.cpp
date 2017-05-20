#include "buttonManager.h"
#include "arduino.h"
#define SAMPLE_TIME 20
buttonManager::buttonManager(uint8_t pin, uint8_t key) {
  buttonManager(pin, key, false);
}
buttonManager::buttonManager(uint8_t pin, uint8_t key, uint8_t isHold)
  : mPin(pin), mKey(key), prevTime(0), sReg(0), mHold(isHold) {}
void buttonManager::init()
{
  pinMode(mPin, INPUT_PULLUP);
}
// This function uses a shift register for button debouncing
// it keeps a history of 3 bits, and if it sees high, low, low
// it registers that the button has been pressed (reversed logic)
bool buttonManager::checkPin() {
  // sample every SAMPLE_TIME
  if (millis() > (prevTime + SAMPLE_TIME)) {
    prevTime = millis();
    //move the prev value over (forms the history)
    sReg = sReg << 1;
    //update the lsb to current value
    sReg |= (digitalRead(mPin) ? 0x1 : 0);
    // get rid of any bits higher than 3
    sReg &= 0x7;

  }
  else if (!mHold)
    return false;
  //high low low
  if (sReg == mKey)
    return true;
  return false;
}
