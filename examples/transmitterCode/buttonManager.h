#ifndef BUTTON_MANAGER
#define BUTTON_MANAGER
#include <stdint.h>
// debounce time for buttons
#define debounceTime 10
class buttonManager {
  public:
    buttonManager(uint8_t pin, uint8_t key);
    void init();
    bool checkPin();
  private:
    uint8_t mPin;
    // shift register
    uint8_t sReg;
    uint8_t mKey;
    // keeps track of prev time for sampling purposes
    unsigned long prevTime;
};
#endif
