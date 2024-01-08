
#include "HidMixer/BluefruitConnectionManagerDetail/CentralManager.h"
#include "HidMixer/BluefruitConnectionManagerDetail/central_profiles/Addr.h"

namespace HidMixer::BluefruitConnectionManagerDetail
{

  CentralManager::ProfileInfo CentralManager::_profile_info_list[CENTRAL_PROFILE_INFO_LIST_COUNT] = {
      {nullptr, BLE_CONN_HANDLE_INVALID, false},
      {nullptr, BLE_CONN_HANDLE_INVALID, false},
      {nullptr, BLE_CONN_HANDLE_INVALID, false},
  };
  CentralManager::connect_callback_t CentralManager::_connect_callback = nullptr;
  CentralManager::disconnect_callback_t CentralManager::_disconnect_callback = nullptr;

  void CentralManager::begin()
  {
    Bluefruit.Central.setConnectCallback(connect_callback);
    Bluefruit.Central.setDisconnectCallback(disconnect_callback);

    Bluefruit.Scanner.setRxCallback(scan_callback);
    Bluefruit.Scanner.restartOnDisconnect(false);
    Bluefruit.Scanner.setInterval(8, 4); // in unit of 0.625 ms
  }

  void CentralManager::setProfile0(BLECentralProfile *profile)
  {
    _profile_info_list[0].profile = profile;
    resetActiveScan();
    resetIoCapsDisplay();
  }

  void CentralManager::setProfile1(BLECentralProfile *profile)
  {
    _profile_info_list[1].profile = profile;
    resetActiveScan();
    resetIoCapsDisplay();
  }

  void CentralManager::setProfile2(BLECentralProfile *profile)
  {
    _profile_info_list[2].profile = profile;
    resetActiveScan();
    resetIoCapsDisplay();
  }

  void CentralManager::start()
  {
    ada_callback(nullptr, 0, start_dfr);
  }

  void CentralManager::setConnectCallback(connect_callback_t callback)
  {
    _connect_callback = callback;
  }

  void CentralManager::setDisconnectCallback(disconnect_callback_t callback)
  {
    _disconnect_callback = callback;
  }

  void CentralManager::resetActiveScan()
  {
    Bluefruit.Scanner.useActiveScan(false);
    for (size_t i = 0; i < CENTRAL_PROFILE_INFO_LIST_COUNT; i++)
    {
      if (_profile_info_list[i].profile != nullptr && _profile_info_list[i].profile->needsActiveScan())
      {
        Bluefruit.Scanner.useActiveScan(true);
        return;
      }
    }
  }

  void CentralManager::resetIoCapsDisplay()
  {
    Bluefruit.Security.setIOCaps(false, false, false);
    for (size_t i = 0; i < CENTRAL_PROFILE_INFO_LIST_COUNT; i++)
    {
      if (_profile_info_list[i].profile != nullptr && _profile_info_list[i].profile->needsIoCapsDisplay())
      {
        Bluefruit.Security.setIOCaps(true, false, false);
        return;
      }
    }
  }

  bool CentralManager::needsScan()
  {
    for (size_t i = 0; i < CENTRAL_PROFILE_INFO_LIST_COUNT; i++)
    {
      if (_profile_info_list[i].profile != nullptr &&
          _profile_info_list[i].conn_handle == BLE_CONN_HANDLE_INVALID)
      {
        return true;
      }
    }
    return false;
  }

  void CentralManager::start_dfr()
  {
    if (needsScan() && !Bluefruit.Scanner.isRunning())
    {
      Bluefruit.Scanner.start(0);
    }
  }

  void CentralManager::scan_callback(ble_gap_evt_adv_report_t *report)
  {
    for (size_t i = 0; i < CENTRAL_PROFILE_INFO_LIST_COUNT; i++)
    {
      if (_profile_info_list[i].profile != nullptr &&
          _profile_info_list[i].conn_handle == BLE_CONN_HANDLE_INVALID &&
          _profile_info_list[i].profile->canConnect(report))
      {
        Bluefruit.Central.connect(report);
        return;
      }
    }

    if (needsScan())
    {
      Bluefruit.Scanner.resume();
    }
  }

  void CentralManager::connect_callback(uint16_t conn_handle)
  {
    BLEConnection *conn = Bluefruit.Connection(conn_handle);

    for (size_t i = 0; i < CENTRAL_PROFILE_INFO_LIST_COUNT; i++)
    {
      if (_profile_info_list[i].profile != nullptr &&
          _profile_info_list[i].conn_handle == BLE_CONN_HANDLE_INVALID &&
          _profile_info_list[i].profile->discover(conn_handle))
      {
        _profile_info_list[i].conn_handle = conn_handle;
        conn->requestPairing();
        return;
      }
    }

    conn->disconnect();
  }

  void CentralManager::pairing_passkey_callback(uint16_t conn_handle, uint8_t const passkey[6])
  {
    for (size_t i = 0; i < CENTRAL_PROFILE_INFO_LIST_COUNT; i++)
    {
      if (_profile_info_list[i].profile != nullptr &&
          _profile_info_list[i].conn_handle == conn_handle)
      {
        char profile_name[] = "[Central] Profile 0";
        profile_name[18] += i;
        Serial.print(profile_name);
        Serial.println(" Device Pairing Passkey");
        Serial.printf("%.6s\n", passkey);
        Serial.flush();
        return;
      }
    }
  }

  void CentralManager::pairing_complete_callback(uint16_t conn_handle, uint8_t auth_status)
  {
    BLEConnection *conn = Bluefruit.Connection(conn_handle);

    for (size_t i = 0; i < CENTRAL_PROFILE_INFO_LIST_COUNT; i++)
    {
      if (_profile_info_list[i].profile != nullptr &&
          _profile_info_list[i].conn_handle == conn_handle)
      {
        char profile_name[] = "[Central] Profile 0";
        profile_name[18] += i;
        Serial.print(profile_name);
        if (auth_status == BLE_GAP_SEC_STATUS_SUCCESS)
        {
          Serial.println(" Device Pairing Success.");
          Serial.flush();
          _profile_info_list[i].profile->successPairing(conn->getPeerAddr().addr);
        }
        else
        {
          Serial.println(" Device Pairing Failure.");
          Serial.flush();
          // disconnect
          conn->disconnect();
        }

        return;
      }
    }
  }

  void CentralManager::disconnect_callback(uint16_t conn_handle, uint8_t reason)
  {
    for (size_t i = 0; i < CENTRAL_PROFILE_INFO_LIST_COUNT; i++)
    {
      if (_profile_info_list[i].profile != nullptr &&
          _profile_info_list[i].conn_handle == conn_handle)
      {
        _profile_info_list[i].conn_handle = BLE_CONN_HANDLE_INVALID;
        if (_disconnect_callback != nullptr && _profile_info_list[i].reached_connect_callback_process)
        {
          _disconnect_callback(i, reason);
        }
        _profile_info_list[i].reached_connect_callback_process = false;
        break;
      }
    }

    if (needsScan() && !Bluefruit.Scanner.isRunning())
    {
      Bluefruit.Scanner.start(0);
    }
  }

  void CentralManager::connection_secured_callback(uint16_t conn_handle)
  {
    BLEConnection *conn = Bluefruit.Connection(conn_handle);

    if (!conn->secured())
    {
      // It is possible that connection is still not secured by this time.
      // This happens (central only) when we try to encrypt connection using stored bond keys
      // but peer reject it (probably it remove its stored key).
      // Therefore we will request an pairing again --> callback again when encrypted
      conn->requestPairing();
    }
    else
    {
      for (size_t i = 0; i < CENTRAL_PROFILE_INFO_LIST_COUNT; i++)
      {
        if (_profile_info_list[i].profile != nullptr &&
            _profile_info_list[i].conn_handle == conn_handle)
        {
          _profile_info_list[i].profile->securedConnect();
          if (_connect_callback != nullptr)
          {
            _connect_callback(i);
          }
          _profile_info_list[i].reached_connect_callback_process = true;
          break;
        }
      }

      conn->requestPHY();

      if (needsScan() && !Bluefruit.Scanner.isRunning())
      {
        Bluefruit.Scanner.start(0);
      }
    }
  }

  void CentralManager::clearBond()
  {
    ada_callback(nullptr, 0, clearBond_dfr);
  }

  void CentralManager::clearBond_dfr()
  {
    AddrStore.clear();
    bond_clear_cntr();

    for (size_t i = 0; i < CENTRAL_PROFILE_INFO_LIST_COUNT; i++)
    {
      if (_profile_info_list[i].profile != nullptr &&
          _profile_info_list[i].conn_handle != BLE_CONN_HANDLE_INVALID)
      {
        Bluefruit.disconnect(_profile_info_list[i].conn_handle);
      }
    }
  }
}
