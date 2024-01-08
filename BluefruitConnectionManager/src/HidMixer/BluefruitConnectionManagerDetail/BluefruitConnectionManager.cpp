
#include "HidMixer/BluefruitConnectionManagerDetail/BluefruitConnectionManager.h"

namespace HidMixer::BluefruitConnectionManagerDetail
{

  void BluefruitConnectionManagerClass::begin()
  {
    Central.begin();
    Periph.begin();

    Serial.begin(115200);

    Bluefruit.Security.setIOCaps(false, false, false); // display = false, yes/no = false, keyboard = false
    Bluefruit.Security.setPairPasskeyCallback(pairing_passkey_callback);
    Bluefruit.Security.setPairCompleteCallback(pairing_complete_callback);
    Bluefruit.Security.setSecuredCallback(connection_secured_callback);
  }

  bool BluefruitConnectionManagerClass::pairing_passkey_callback(uint16_t conn_handle, uint8_t const passkey[6], bool match_request)
  {
    // Numberic Comparison is currently not supported
    if (match_request)
    {
      return false;
    }

    Periph.pairing_passkey_callback(conn_handle, passkey);
    Central.pairing_passkey_callback(conn_handle, passkey);
    return true;
  }

  void BluefruitConnectionManagerClass::pairing_complete_callback(uint16_t conn_handle, uint8_t auth_status)
  {
    Periph.pairing_complete_callback(conn_handle, auth_status);
    Central.pairing_complete_callback(conn_handle, auth_status);
  }

  void BluefruitConnectionManagerClass::connection_secured_callback(uint16_t conn_handle)
  {
    Periph.connection_secured_callback(conn_handle);
    Central.connection_secured_callback(conn_handle);
  }

}
