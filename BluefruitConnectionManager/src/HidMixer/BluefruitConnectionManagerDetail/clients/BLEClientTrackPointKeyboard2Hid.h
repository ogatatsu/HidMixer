
#pragma once

#include "BLEClientCharacteristic.h"
#include "BLEClientService.h"

namespace HidMixer::BluefruitConnectionManagerDetail
{

  class BLEClientTrackPointKeyboard2Hid : public BLEClientService
  {
  public:
    // Callback Signatures
    using keyboard_callback_t = void (*)(uint8_t modifiers, uint8_t key_codes[6]);
    using trackpoint_callback_t = void (*)(uint8_t buttons, int8_t x, int8_t y, int8_t wheel);
    using consumer_callback_t = void (*)(uint16_t usage_code);
    using vendor_callback_t = void (*)(uint16_t usage_code);

    BLEClientTrackPointKeyboard2Hid();

    bool begin() override;
    bool discover(uint16_t conn_handle) override;

    bool enableKeyboard();
    bool disableKeyboard();

    bool enableTrackpoint();
    bool disableTrackpoint();

    bool enableConsumer();
    bool disableConsumer();

    bool enableVendor();
    bool disableVendor();

    bool setKeyboardLed(uint8_t leds_bitmap);

    // Report callback
    void setKeyboardReportCallback(keyboard_callback_t fp);
    void setTrackpointReportCallback(trackpoint_callback_t fp);
    void setConsumerReportCallback(consumer_callback_t fp);
    void setVendorReportCallback(vendor_callback_t fp);

  private:
    keyboard_callback_t _keyboard_cb;
    trackpoint_callback_t _trackpoint_cb;
    consumer_callback_t _consumer_cb;
    vendor_callback_t _vendor_cb;

    BLEClientCharacteristic _keyboard_input;
    BLEClientCharacteristic _trackpoint_input;
    BLEClientCharacteristic _consumer_input;
    BLEClientCharacteristic _unknown1;
    BLEClientCharacteristic _unknown2;
    BLEClientCharacteristic _vendor_input;
    BLEClientCharacteristic _unknown3;
    BLEClientCharacteristic _keyboard_output;

    void _handle_keyboard_input(uint8_t *data, uint16_t len);
    void _handle_trackpoint_input(uint8_t *data, uint16_t len);
    void _handle_consumer_input(uint8_t *data, uint16_t len);
    void _handle_vendor_input(uint8_t *data, uint16_t len);

    static void keyboard_client_notify_cb(BLEClientCharacteristic *chr, uint8_t *data, uint16_t len);
    static void trackpoint_client_notify_cb(BLEClientCharacteristic *chr, uint8_t *data, uint16_t len);
    static void consumer_client_notify_cb(BLEClientCharacteristic *chr, uint8_t *data, uint16_t len);
    static void vendor_client_notify_cb(BLEClientCharacteristic *chr, uint8_t *data, uint16_t len);

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
    };

    struct consumer_report_t
    {
      uint16_t usage_code;
    };

    struct vendor_report_t
    {
      uint16_t usage_code;
    };
#pragma pack()
  };

}
