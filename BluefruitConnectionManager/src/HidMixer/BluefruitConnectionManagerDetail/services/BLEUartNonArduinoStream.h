
#pragma once

#include "BLECharacteristic.h"
#include "BLEService.h"

namespace HidMixer::BluefruitConnectionManagerDetail
{

  class BLEUartNonArduinoStream : public BLEService
  {
  public:
    using rx_callback_t = void (*)(uint8_t *data, uint16_t len);
    using notify_callback_t = void (*)(bool enabled);

    BLEUartNonArduinoStream();

    virtual err_t begin(void);
    bool notifyEnabled(void);
    void setRxCallback(rx_callback_t fp);
    void setNotifyCallback(notify_callback_t fp);
    uint16_t write(const uint8_t *content, uint16_t len);

  protected:
    BLECharacteristic _txd;
    BLECharacteristic _rxd;

    // Callbacks
    rx_callback_t _rx_cb;
    notify_callback_t _notify_cb;

    // Static Method for callbacks
    static void bleuart_rxd_cb(uint16_t conn_hdl, BLECharacteristic *chr, uint8_t *data, uint16_t len);
    static void bleuart_txd_cccd_cb(uint16_t conn_hdl, BLECharacteristic *chr, uint16_t value);
  };

}
