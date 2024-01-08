
#pragma once

#include "HidMixer/BluefruitConnectionManagerDetail/CentralManager.h"
#include "HidMixer/BluefruitConnectionManagerDetail/PeripheralManager.h"

namespace HidMixer::BluefruitConnectionManagerDetail
{

  class BluefruitConnectionManagerClass
  {
  public:
    static void begin();

    static CentralManager Central;
    static PeripheralManager Periph;

  private:
    static void connection_secured_callback(uint16_t conn_handle);
    static bool pairing_passkey_callback(uint16_t conn_handle, uint8_t const passkey[6], bool match_request);
    static void pairing_complete_callback(uint16_t conn_handle, uint8_t auth_status);
  };

  extern BluefruitConnectionManagerClass BluefruitConnectionManager;

}
