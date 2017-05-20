#ifndef BUTTON_MANAGER
#define BUTTON_MANAGER
#include <stdint.h>
// debounce time for buttons
#define debounceTime 10
class buttonManager {
  public:
    buttonManager(uint8_t pin, uint8_t key);
    buttonManager::buttonManager(uint8_t pin, uint8_t key, uint8_t isHold);
    void init();
    bool checkPin();
  private:
    uint8_t mPin;
    // shift register
    uint8_t sReg;
    uint8_t mKey;
    uint8_t mHold;
    // keeps track of prev time for sampling purposes
    unsigned long prevTime;
};
#endif
