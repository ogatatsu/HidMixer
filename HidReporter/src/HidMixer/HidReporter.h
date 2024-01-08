
#pragma once

#include <stdint.h>

namespace HidMixer
{

  class HidReporter
  {
  public:
    virtual bool keyboardReport(uint8_t modifiers, uint8_t key_codes[6]) = 0;
    virtual bool mouseReport(uint8_t buttons, int16_t x, int16_t y, int8_t wheel, int8_t pan) = 0;
    virtual bool consumerReport(uint16_t usage_code) = 0;
    virtual bool systemControlReport(uint8_t usage_code) = 0;
    virtual bool waitReady() = 0;
  };

}
