
#pragma once

#include "HidMixer/HidReporter.h"
#include "bluefruit.h"

namespace HidMixer::BluefruitConnectionManagerDetail
{

  class BLEHid : public BLEHidGeneric, public HidReporter
  {
  public:
    using keyboard_led_callback_t = void (*)(uint8_t leds_bitmap);

    BLEHid();
    err_t begin();

    bool keyboardReport(uint8_t modifiers, uint8_t key_codes[6]) override;
    bool mouseReport(uint8_t buttons, int16_t x, int16_t y, int8_t wheel, int8_t pan) override;
    bool consumerReport(uint16_t usage_code) override;
    bool systemControlReport(uint8_t usage_code) override;
    bool waitReady() override;
    void setKeyboardLedCallback(keyboard_led_callback_t cb);

  private:
    keyboard_led_callback_t _keyboard_led_cb;

    static void keyboard_output_cb(uint16_t conn_hdl, BLECharacteristic *chr, uint8_t *data, uint16_t len);
  };

}
