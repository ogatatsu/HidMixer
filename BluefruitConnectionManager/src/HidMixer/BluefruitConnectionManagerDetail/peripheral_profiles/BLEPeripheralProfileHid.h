
#pragma once

#include "HidMixer/BluefruitConnectionManagerDetail/BLEPeripheralProfile.h"
#include "HidMixer/BluefruitConnectionManagerDetail/services/BLEHid.h"
#include "services/BLEBas.h"
#include "services/BLEDis.h"

namespace HidMixer::BluefruitConnectionManagerDetail
{

  class BLEPeripheralProfileHid : public BLEPeripheralProfile
  {
  public:
    BLEDis Dis;
    BLEBas Bas;
    BLEHid Hid;

    bool begin() override
    {
      VERIFY_STATUS(Dis.begin());
      VERIFY_STATUS(Bas.begin());
      VERIFY_STATUS(Hid.begin());
      return true;
    };

    BLEPeripheralProfileHid(uint16_t appearance = BLE_APPEARANCE_GENERIC_HID,
                            uint16_t connection_interval = 6,
                            uint16_t slave_latency = 30,
                            uint16_t supervision_timeout = 200)
        : _appearance(appearance),
          _connection_interval(connection_interval),
          _slave_latency(slave_latency),
          _supervision_timeout(supervision_timeout)
    {
    }

    uint16_t getAppearance() override { return _appearance; };
    BLEService &getService() override { return Hid; };
    uint16_t getConnectionInterval() override { return _connection_interval; };
    uint16_t getSlaveLatency() override { return _slave_latency; };
    uint16_t getSupervisionTimeout() override { return _supervision_timeout; };
    HidReporter *getHidReporter() { return &Hid; }

  private:
    uint16_t _appearance;
    uint16_t _connection_interval;
    uint16_t _slave_latency;
    uint16_t _supervision_timeout;
  };

}
