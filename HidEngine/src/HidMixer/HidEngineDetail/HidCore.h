
#pragma once

#include "FreeRTOS.h"
#include "HidMixer/HidEngineDetail/KeyCode.h"
#include "HidMixer/HidReporter.h"

namespace HidMixer::HidEngineDetail
{
  class HidCore
  {
  public:
    static void setReporter(HidReporter *hid_reporter);
    static void waitReady();

    // Keyboard API
    static void keyPress(CharacterKeyCode character_key);
    static void keyRelease(CharacterKeyCode character_key);
    static void modifiersPress(ModifiersCode modifiers);
    static void modifiersRelease(ModifiersCode modifiers);
    static bool isModifiersSet();

    // Consumer Control API
    static void consumerControlPress(ConsumerControlCode usage_code);
    static void consumerControlRelease();

    // System Control API
    static void systemControlPress(SystemControlCode usage_code);
    static void systemControlRelease();

    // Poinding Device API
    static void pointerMove(int16_t x, int16_t y);
    static void pointingScroll(int8_t scroll, int8_t pan);
    static void pointingButtonsPress(PointingButtonsCode buttons);
    static void pointingButtonsRelease(PointingButtonsCode buttons);

  private:
    static void setKey(CharacterKeyCode character_key);
    static void unsetKey(CharacterKeyCode character_key);
    static void setModifiers(ModifiersCode modifiers);
    static void unsetModifiers(ModifiersCode modifiers);
    static void setPointingButtons(PointingButtonsCode buttons);
    static void unsetPointingButtons(PointingButtonsCode buttons);
    static void sendKeyReport();
    static void sendPointingButtonsReport();

    static HidReporter *_hid_reporter;

    static uint8_t _pressed_keys[7];
    static uint8_t _prev_sent_keys[6];
    static uint8_t _key_counters[256];

    static uint8_t _pressed_modifiers;
    static uint8_t _prev_sent_modifiers;
    static uint8_t _modifier_counters[8];

    static uint8_t _pressed_pointing_buttons;
    static uint8_t _prev_sent_pointing_buttons;
    static uint8_t _pointing_button_counters[5];

    static TickType_t _last_send_ticks;
  };

  extern HidCore Hid;

}
