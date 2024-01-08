
#pragma once

#include "bluefruit.h"

namespace HidMixer::BluefruitConnectionManagerDetail
{

  class BLECentralProfile
  {
  public:
    virtual bool begin() = 0;
    virtual bool needsActiveScan() = 0;
    virtual bool canConnect(ble_gap_evt_adv_report_t *report) = 0;
    virtual bool discover(uint16_t conn_handle) = 0;
    virtual void successPairing(uint8_t peer_addr[6]) = 0;
    virtual void securedConnect() = 0;
    virtual bool needsIoCapsDisplay() = 0;
  };
  
}
