
#pragma once

#include "HidMixer/BluefruitConnectionManagerDetail/BLECentralProfile.h"
#include "HidMixer/BluefruitConnectionManagerDetail/central_profiles/Addr.h"
#include "HidMixer/BluefruitConnectionManagerDetail/clients/BLEClientRelaconHid.h"
#include "clients/BLEClientBas.h"
#include "clients/BLEClientDis.h"
#include "etl/optional.h"

namespace HidMixer::BluefruitConnectionManagerDetail
{

  class BLECentralProfileRelacon : public BLECentralProfile
  {
  public:
    BLEClientDis Dis;
    BLEClientBas Bas;
    BLEClientRelaconHid Hid;

    BLECentralProfileRelacon(const char *path);
    bool begin() override;
    bool needsActiveScan() override { return false; }
    bool canConnect(ble_gap_evt_adv_report_t *report) override;
    bool discover(uint16_t conn_handle) override;
    void successPairing(uint8_t addr[6]) override;
    void securedConnect() override;
    bool needsIoCapsDisplay() override { return false; }

  private:
    const char *_path;
    etl::optional<Addr> _peer_addr;
  };

};
