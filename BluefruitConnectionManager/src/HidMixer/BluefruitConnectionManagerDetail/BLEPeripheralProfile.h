
#pragma once

#include "bluefruit.h"

namespace HidMixer::BluefruitConnectionManagerDetail
{

  class BLEPeripheralProfile
  {
  public:
    virtual bool begin() = 0;
    virtual uint16_t getAppearance() = 0;
    virtual BLEService &getService() = 0;
    virtual uint16_t getConnectionInterval() = 0;
    virtual uint16_t getSlaveLatency() = 0;
    virtual uint16_t getSupervisionTimeout() = 0;
  };

}
