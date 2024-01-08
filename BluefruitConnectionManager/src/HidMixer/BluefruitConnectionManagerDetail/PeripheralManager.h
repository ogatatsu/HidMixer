
#pragma once

#include "HidMixer/BluefruitConnectionManagerDetail/BLEPeripheralProfile.h"
#include "HidMixer/MemStore.h"
#include <atomic>

namespace HidMixer::BluefruitConnectionManagerDetail
{

  class PeripheralManager
  {
    friend class BluefruitConnectionManagerClass;

  public:
    using connect_callback_t = void (*)(void);
    using disconnect_callback_t = void (*)(uint8_t reason);
    using cannot_connect_callback_t = void (*)(void);

    static void setProfile(BLEPeripheralProfile *profile);
    static void start();
    static void stop();
    static bool isRunning();
    static void setConnectCallback(connect_callback_t callback);
    static void setDisconnectCallback(disconnect_callback_t callback);
    static void setCannotConnectCallback(cannot_connect_callback_t callback);
    static void clearBond();

  private:
    static void begin();
    static void start_dfr();
    static void stop_dfr();
    static bool restartAdvertising();
    static void makeAdvertisingData();
    static void adv_stop_callback();
    static void connect_callback(uint16_t conn_handle);
    static void pairing_passkey_callback(uint16_t conn_handle, uint8_t const passkey[6]);
    static void pairing_complete_callback(uint16_t conn_handle, uint8_t auth_status);
    static void connection_secured_callback(uint16_t conn_handle);
    static void disconnect_callback(uint16_t conn_handle, uint8_t reason);
    static void clearBond_dfr();

    static BLEPeripheralProfile *_profile;
    static connect_callback_t _connect_callback;
    static disconnect_callback_t _disconnect_callback;
    static cannot_connect_callback_t _cannot_connect_callback;
    static uint16_t _conn_handle;
    static std::atomic_bool _is_running;
    static bool _is_paired;
    static MemStore _paired_flag_store;
    static bool _reached_connect_callback_process;
  };

}
