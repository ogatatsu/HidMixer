
#pragma once

#include "HidMixer/BluefruitConnectionManagerDetail/BLEPeripheralProfile.h"
#include "HidMixer/BluefruitConnectionManagerDetail/services/BLEUartNonArduinoStream.h"
#include "services/BLEBas.h"
#include "services/BLEDis.h"

namespace HidMixer::BluefruitConnectionManagerDetail
{

  class BLEPeripheralProfileUart : public BLEPeripheralProfile
  {
  public:
    BLEDis Dis;
    BLEBas Bas;
    BLEUartNonArduinoStream Uart;

    bool begin() override
    {
      VERIFY_STATUS(Dis.begin());
      VERIFY_STATUS(Bas.begin());
      VERIFY_STATUS(Uart.begin());
      return true;
    };

    BLEPeripheralProfileUart(uint16_t appearance = BLE_APPEARANCE_UNKNOWN,
                             uint16_t connection_interval = 6,
                             uint16_t slave_latency = 65,
                             uint16_t supervision_timeout = 100)
        : _appearance(appearance),
          _connection_interval(connection_interval),
          _slave_latency(slave_latency),
          _supervision_timeout(supervision_timeout)
    {
    }

    uint16_t getAppearance() override { return _appearance; };
    BLEService &getService() override { return Uart; };
    uint16_t getConnectionInterval() override { return _connection_interval; };
    uint16_t getSlaveLatency() override { return _slave_latency; };
    uint16_t getSupervisionTimeout() override { return _supervision_timeout; };

  private:
    uint16_t _appearance;
    uint16_t _connection_interval;
    uint16_t _slave_latency;
    uint16_t _supervision_timeout;
  };

}
