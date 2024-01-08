
#pragma once

#include "BLEClientCharacteristic.h"
#include "BLEClientService.h"

namespace HidMixer::BluefruitConnectionManagerDetail
{

  class BLEClientHHKBStudioHid : public BLEClientService
  {
  public:
    // Callback Signatures
    using keyboard_callback_t = void (*)(uint8_t modifiers, uint8_t key_codes[6]);
    using trackpoint_callback_t = void (*)(uint8_t buttons, int8_t x, int8_t y, int8_t wheel, int8_t pan);

    BLEClientHHKBStudioHid();

    bool begin() override;
    bool discover(uint16_t conn_handle) override;

    bool enableKeyboard();
    bool disableKeyboard();

    bool enableTrackpoint();
    bool disableTrackpoint();

    // Report callback
    void setKeyboardReportCallback(keyboard_callback_t fp);
    void setTrackpointReportCallback(trackpoint_callback_t fp);

  private:
    keyboard_callback_t _keyboard_cb;
    trackpoint_callback_t _trackpoint_cb;

    BLEClientCharacteristic _keyboard_input;
    BLEClientCharacteristic _trackpoint_input;

    void _handle_keyboard_input(uint8_t *data, uint16_t len);
    void _handle_trackpoint_input(uint8_t *data, uint16_t len);

    static void keyboard_client_notify_cb(BLEClientCharacteristic *chr, uint8_t *data, uint16_t len);
    static void trackpoint_client_notify_cb(BLEClientCharacteristic *chr, uint8_t *data, uint16_t len);

#pragma pack(1)
    struct keyboard_report_t
    {
      uint8_t modifiers;
      uint8_t reserved;
      uint8_t key_codes[6];
    };

    struct trackpoint_report_t
    {
      uint8_t buttons;
      int8_t x;
      int8_t y;
      int8_t wheel;
      int8_t pan;
    };
#pragma pack()
  };

}
