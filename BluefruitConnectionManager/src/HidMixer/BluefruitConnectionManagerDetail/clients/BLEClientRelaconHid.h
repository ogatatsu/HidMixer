
#pragma once

#include "BLEClientCharacteristic.h"
#include "BLEClientService.h"

namespace HidMixer::BluefruitConnectionManagerDetail
{

  class BLEClientRelaconHid : public BLEClientService
  {
  public:
    // Callback Signatures
    using trackball_callback_t = void (*)(uint8_t buttons, int16_t x, int16_t y, int8_t wheel);
    using consumer_callback_t = void (*)(uint16_t usage_code);

    BLEClientRelaconHid();

    bool begin() override;
    bool discover(uint16_t conn_handle) override;

    // Trackball API
    bool enableTrackball();
    bool disableTrackball();

    // Consumer API
    bool enableConsumer();
    bool disableConsumer();

    // Report callback
    void setTrackballReportCallback(trackball_callback_t fp);
    void setConsumerReportCallback(consumer_callback_t fp);

  private:
    trackball_callback_t _trackball_cb;
    consumer_callback_t _consumer_cb;

    BLEClientCharacteristic _trackball_input;
    BLEClientCharacteristic _consumer_input;

    void _handle_trackball_input(uint8_t *data, uint16_t len);
    void _handle_consumer_input(uint8_t *data, uint16_t len);

    static void trackball_client_notify_cb(BLEClientCharacteristic *chr, uint8_t *data, uint16_t len);
    static void consumer_client_notify_cb(BLEClientCharacteristic *chr, uint8_t *data, uint16_t len);

#pragma pack(1)
    struct trackball_report_t
    {
      uint8_t buttons;
      int16_t x;
      int16_t y;
      int8_t wheel;
      int8_t pan; // not used
    };

    struct consumer_report_t
    {
      uint16_t usage_code;
    };
#pragma pack()
  };

}
