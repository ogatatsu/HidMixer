
#include "HidMixer/BluefruitConnectionManagerDetail/central_profiles/BLECentralProfileRelacon.h"
#include "bluefruit.h"

namespace HidMixer::BluefruitConnectionManagerDetail
{
  BLECentralProfileRelacon::BLECentralProfileRelacon(const char *path) : _path(path), _peer_addr(etl::nullopt)
  {
  }

  bool BLECentralProfileRelacon::begin()
  {
    VERIFY(Dis.begin());
    VERIFY(Bas.begin());
    VERIFY(Hid.begin());

    Addr addr;
    if (AddrStore.load(_path, &addr, sizeof(addr)))
    {
      _peer_addr = addr;
    }

    return true;
  }

  bool BLECentralProfileRelacon::canConnect(ble_gap_evt_adv_report_t *report)
  {
    if (_peer_addr.has_value())
    {
      if (_peer_addr->isSameAddr(report->peer_addr.addr))
      {
        return true;
      }
    }
    else
    {
      uint8_t buf[32];

      if ((report->type.scan_response == 0) &&
          Bluefruit.Scanner.parseReportByType(report, BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME, buf, sizeof(buf)))
      {
        if (memcmp(buf, "ELECOM Relacon", 14) == 0)
        {
          return true;
        }
      }
    }
    return false;
  }

  bool BLECentralProfileRelacon::discover(uint16_t conn_handle)
  {
    BLEConnection *conn = Bluefruit.Connection(conn_handle);

    bool is_intended = false;

    if (_peer_addr.has_value())
    {
      if (_peer_addr->isSameAddr(conn->getPeerAddr().addr))
      {
        is_intended = true;
      }
    }
    else
    {
      char buf[32];
      conn->getPeerName(buf, sizeof(buf));

      if (memcmp(buf, "ELECOM Relacon", 14) == 0)
      {
        is_intended = true;
      }
    }

    if (is_intended)
    {
      VERIFY(Dis.discover(conn_handle));
      VERIFY(Bas.discover(conn_handle));
      VERIFY(Hid.discover(conn_handle));

      return true;
    }

    return false;
  }

  void BLECentralProfileRelacon::successPairing(uint8_t addr[6])
  {
    _peer_addr.emplace(addr);
    AddrStore.save(_path, &_peer_addr.value(), sizeof(Addr));
  }

  void BLECentralProfileRelacon::securedConnect()
  {
    Bas.enableNotify();
    Hid.enableTrackball();
    Hid.enableConsumer();
  }

}
