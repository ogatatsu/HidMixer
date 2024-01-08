
#pragma once

#include "HidMixer/BluefruitConnectionManagerDetail/BLECentralProfile.h"

namespace HidMixer::BluefruitConnectionManagerDetail
{
  constexpr size_t CENTRAL_PROFILE_INFO_LIST_COUNT = 3;

  class CentralManager
  {
    friend class BluefruitConnectionManagerClass;

  public:
    using connect_callback_t = void (*)(size_t profile_num);
    using disconnect_callback_t = void (*)(size_t profile_num, uint8_t reason);

    static void setProfile0(BLECentralProfile *profile);
    static void setProfile1(BLECentralProfile *profile);
    static void setProfile2(BLECentralProfile *profile);
    static void start();
    static void setConnectCallback(connect_callback_t callback);
    static void setDisconnectCallback(disconnect_callback_t callback);
    static void clearBond();

  private:
    static void begin();
    static void resetActiveScan();
    static void resetIoCapsDisplay();
    static void start_dfr();
    static bool needsScan();
    static void scan_callback(ble_gap_evt_adv_report_t *report);
    static void connect_callback(uint16_t conn_handle);
    static void pairing_passkey_callback(uint16_t conn_handle, uint8_t const passkey[6]);
    static void pairing_complete_callback(uint16_t conn_handle, uint8_t auth_status);
    static void connection_secured_callback(uint16_t conn_handle);
    static void disconnect_callback(uint16_t conn_handle, uint8_t reason);
    static void clearBond_dfr();

    struct ProfileInfo
    {
      BLECentralProfile *profile;
      uint16_t conn_handle;
      bool reached_connect_callback_process;
    };

    static ProfileInfo _profile_info_list[CENTRAL_PROFILE_INFO_LIST_COUNT];
    static connect_callback_t _connect_callback;
    static disconnect_callback_t _disconnect_callback;
  };

}
