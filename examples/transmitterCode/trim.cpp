#include "buttonManager.h"
#include "trim.h"
// constructor makes two buttons: one for upTrim one for downTrim
trim::trim(uint8_t upPin, uint8_t downPin)
  : mUpPin(upPin, 0x4, false), mDownPin(downPin, 0x4, false), mValue(0) {
}
// updateValue updates the value for the trim for the struct
void trim::updateValue() {
  if (mUpPin.checkPin())
    mValue++;
  if (mDownPin.checkPin())
    mValue--;
}
void trim::init() {
  mUpPin.init();
  mDownPin.init();
}

