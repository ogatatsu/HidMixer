
#pragma once

#include "HidMixer/HidEngineDetail/HidEngine.h"
#include "consthash/cityhash64.hxx"
#include <new>

namespace HidMixer::UsbActionDetail
{

  //------------------------------------------------------------------+
  // RemoteWakeup
  //------------------------------------------------------------------+
  class RemoteWakeup : public HidEngineDetail::Action
  {
  protected:
    void onPress() override;
  };

  template <uint64_t ID1, uint64_t ID2>
  HidEngineDetail::NotNullActionPtr new_RemoteWakeup()
  {
    static uint8_t buf[sizeof(RemoteWakeup)];
    return new (buf) RemoteWakeup();
  }

}
