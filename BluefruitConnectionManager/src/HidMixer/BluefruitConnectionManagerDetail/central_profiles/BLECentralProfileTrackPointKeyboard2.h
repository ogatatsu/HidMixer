
#pragma once

#include "HidMixer/BluefruitConnectionManagerDetail/BLECentralProfile.h"
#include "HidMixer/BluefruitConnectionManagerDetail/central_profiles/Addr.h"
#include "HidMixer/BluefruitConnectionManagerDetail/clients/BLEClientTrackPointKeyboard2Hid.h"
#include "clients/BLEClientDis.h"
#include "etl/optional.h"

namespace HidMixer::BluefruitConnectionManagerDetail
{

  class BLECentralProfileTrackpointKeyboard2 : public BLECentralProfile
  {
  public:
    BLEClientDis Dis;
    BLEClientTrackPointKeyboard2Hid Hid;

    BLECentralProfileTrackpointKeyboard2(const char *path);
    bool begin() override;
    bool needsActiveScan() override { return true; }
    bool canConnect(ble_gap_evt_adv_report_t *report) override;
    bool discover(uint16_t conn_handle) override;
    void successPairing(uint8_t peer_addr[6]) override;
    void securedConnect() override;
    bool needsIoCapsDisplay() override { return false; }

  private:
    const char *_path;
    etl::optional<Addr> _peer_addr;
  };

}
