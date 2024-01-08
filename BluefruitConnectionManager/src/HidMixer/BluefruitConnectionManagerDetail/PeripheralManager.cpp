
#include "HidMixer/BluefruitConnectionManagerDetail/PeripheralManager.h"

constexpr const char *PAIRED_FLAG_STORE_DIR_NAME = "PrphMng";
constexpr const char *PAIRED_FLAG_STORE_FILE_NAME = "paired";

namespace HidMixer::BluefruitConnectionManagerDetail
{

  BLEPeripheralProfile *PeripheralManager::_profile;
  PeripheralManager::connect_callback_t PeripheralManager::_connect_callback = nullptr;
  PeripheralManager::disconnect_callback_t PeripheralManager::_disconnect_callback = nullptr;
  PeripheralManager::cannot_connect_callback_t PeripheralManager::_cannot_connect_callback = nullptr;
  uint16_t PeripheralManager::_conn_handle = BLE_CONN_HANDLE_INVALID;
  std::atomic_bool PeripheralManager::_is_running(false);
  bool PeripheralManager::_is_paired = false;
  MemStore PeripheralManager::_paired_flag_store(PAIRED_FLAG_STORE_DIR_NAME);
  bool PeripheralManager::_reached_connect_callback_process = false;

  void PeripheralManager::begin()
  {
    Bluefruit.Periph.setConnectCallback(connect_callback);
    Bluefruit.Periph.setDisconnectCallback(disconnect_callback);

    Bluefruit.Advertising.setStopCallback(adv_stop_callback);
    Bluefruit.Advertising.restartOnDisconnect(false);
    Bluefruit.Advertising.setInterval(32, 244); // in unit of 0.625 ms
    Bluefruit.Advertising.setFastTimeout(30);   // number of seconds in fast mode

    bool tmp;
    if (_paired_flag_store.load(PAIRED_FLAG_STORE_FILE_NAME, &tmp, sizeof(bool)))
    {
      _is_paired = tmp;
    }
  }

  void PeripheralManager::setProfile(BLEPeripheralProfile *profile)
  {
    _profile = profile;
  }

  void PeripheralManager::setConnectCallback(connect_callback_t callback)
  {
    _connect_callback = callback;
  }

  void PeripheralManager::setDisconnectCallback(disconnect_callback_t callback)
  {
    _disconnect_callback = callback;
  }

  void PeripheralManager::setCannotConnectCallback(cannot_connect_callback_t callback)
  {
    _cannot_connect_callback = callback;
  }

  void PeripheralManager::start()
  {
    ada_callback(nullptr, 0, start_dfr);
  }

  void PeripheralManager::stop()
  {
    ada_callback(nullptr, 0, stop_dfr);
  }

  void PeripheralManager::start_dfr()
  {
    if (_is_running || _profile == nullptr)
    {
      return;
    }

    if (restartAdvertising())
    {
      _is_running = true;
    };
  }

  void PeripheralManager::stop_dfr()
  {
    if (Bluefruit.Advertising.isRunning())
    {
      Bluefruit.Advertising.stop();
    }

    if (Bluefruit.Periph.connected())
    {
      Bluefruit.disconnect(Bluefruit.connHandle());
    }

    _is_running = false;
  }

  bool PeripheralManager::isRunning()
  {
    return _is_running;
  }

  void PeripheralManager::makeAdvertisingData()
  {
    Bluefruit.Advertising.clearData();

    if (_is_paired)
    {
      Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED);
    }
    else
    {
      Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    }

    // Advertising packet
    Bluefruit.Advertising.addAppearance(_profile->getAppearance());
    Bluefruit.Advertising.addService(_profile->getService());

    // Secondary Scan Response packet (optional)
    // Since there is no room for 'Name' in Advertising packet
    Bluefruit.ScanResponse.addName();
  }

  bool PeripheralManager::restartAdvertising()
  {
    if (Bluefruit.Advertising.isRunning())
    {
      Bluefruit.Advertising.stop();
    }

    makeAdvertisingData();

    if (_is_paired)
    {
      return Bluefruit.Advertising.start(60);
    }
    else
    {
      return Bluefruit.Advertising.start();
    }
  }

  void PeripheralManager::adv_stop_callback()
  {
    _is_running = false;

    if (_cannot_connect_callback != nullptr)
    {
      _cannot_connect_callback();
    }
  }

  void PeripheralManager::connect_callback(uint16_t conn_handle)
  {
    BLEConnection *conn = Bluefruit.Connection(conn_handle);

    if (_is_paired)
    {
      bond_keys_t tmp;
      if (!conn->loadBondKey(&tmp))
      {
        conn->disconnect();
      }
    }
    else
    {
      conn->requestPairing();
    }

    _conn_handle = conn_handle;
  }

  void PeripheralManager::disconnect_callback(uint16_t conn_handle, uint8_t reason)
  {
    _conn_handle = BLE_CONN_HANDLE_INVALID;

    if (_disconnect_callback != nullptr && _reached_connect_callback_process)
    {
      _disconnect_callback(reason);
    }
    _reached_connect_callback_process = false;

    if (_is_running)
    {
      restartAdvertising();
    }
  }

  void PeripheralManager::pairing_passkey_callback(uint16_t conn_handle, uint8_t const passkey[6])
  {
    if (_conn_handle == conn_handle)
    {
      Serial.println("[Peripheral] Device Pairing Passkey");
      Serial.printf("%.6s\n", passkey);
      Serial.flush();
    }
  }

  void PeripheralManager::pairing_complete_callback(uint16_t conn_handle, uint8_t auth_status)
  {
    if (_conn_handle == conn_handle)
    {
      if (auth_status == BLE_GAP_SEC_STATUS_SUCCESS)
      {
        Serial.println("[Peripheral] Device Pairing Success.");
        Serial.flush();

        _is_paired = true;
        _paired_flag_store.save(PAIRED_FLAG_STORE_FILE_NAME, &_is_paired, sizeof(bool));
      }
      else
      {
        Serial.println("[Peripheral] Device Pairing Failure.");
        Serial.flush();
      }
    }
  }

  void PeripheralManager::connection_secured_callback(uint16_t conn_handle)
  {
    if (conn_handle == _conn_handle)
    {
      if (_connect_callback != nullptr)
      {
        _connect_callback();
      }
      _reached_connect_callback_process = true;

      BLEConnection *conn = Bluefruit.Connection(conn_handle);
      conn->requestPHY();
      conn->requestConnectionParameter(_profile->getConnectionInterval(), _profile->getSlaveLatency(), _profile->getSupervisionTimeout());
    }
  }

  void PeripheralManager::clearBond()
  {
    ada_callback(nullptr, 0, clearBond_dfr);
  }

  void PeripheralManager::clearBond_dfr()
  {
    _is_paired = false;
    _paired_flag_store.clear();
    bond_clear_prph();

    if (Bluefruit.Periph.connected())
    {
      Bluefruit.disconnect(Bluefruit.connHandle());
    }
    else if (_is_running)
    {
      restartAdvertising();
    }
  }

}
