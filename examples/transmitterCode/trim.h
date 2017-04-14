#ifndef TRIM_H
#define TRIM_H
#include <stdint.h>
#include "buttonManager.h"

class trim {
  public:
    trim(uint8_t upPin, uint8_t downPin);
    void init();
    void updateValue();
    int8_t getValue() {
      return mValue;
    }
  private:
    int8_t mValue;
    buttonManager mUpPin;
    buttonManager mDownPin;
};
#endif
