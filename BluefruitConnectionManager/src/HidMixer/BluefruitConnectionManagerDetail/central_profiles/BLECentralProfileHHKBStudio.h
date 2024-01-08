
#pragma once

#include "HidMixer/BluefruitConnectionManagerDetail/BLECentralProfile.h"
#include "HidMixer/BluefruitConnectionManagerDetail/central_profiles/Addr.h"
#include "HidMixer/BluefruitConnectionManagerDetail/clients/BLEClientHHKBStudioHid.h"
#include "clients/BLEClientBas.h"
#include "clients/BLEClientDis.h"
#include "etl/optional.h"

namespace HidMixer::BluefruitConnectionManagerDetail
{

  class BLECentralProfileHHKBStudio : public BLECentralProfile
  {
  public:
    BLEClientDis Dis;
    BLEClientBas Bas;
    BLEClientHHKBStudioHid Hid;

    BLECentralProfileHHKBStudio(const char *path);
    bool begin() override;
    bool needsActiveScan() override { return false; }
    bool canConnect(ble_gap_evt_adv_report_t *report) override;
    bool discover(uint16_t conn_handle) override;
    void successPairing(uint8_t peer_addr[6]) override;
    void securedConnect() override;
    // HHKB StudioはPasskeyEntry必須、BLE_GAP_IO_CAPS_NONEの場合ペアリングはできても動かない。
    bool needsIoCapsDisplay() override { return true; }

  private:
    const char *_path;
    etl::optional<Addr> _peer_addr;
  };

}
