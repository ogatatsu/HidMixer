
#pragma once

#include "HidMixer/HidEngineDetail/HidEngine.h"
#include "consthash/cityhash64.hxx"
#include <new>

namespace HidMixer::BleActionDetail
{

  //------------------------------------------------------------------+
  // ResetCentral
  //------------------------------------------------------------------+
  class ResetCentral : public HidEngineDetail::Action
  {
  protected:
    void onPress() override;

  public:
  };

  template <uint64_t ID1, uint64_t ID2>
  HidEngineDetail::NotNullActionPtr new_ResetCentral()
  {
    static uint8_t buf[sizeof(ResetCentral)];
    return new (buf) ResetCentral();
  }

  //------------------------------------------------------------------+
  // ResetPeripheral
  //------------------------------------------------------------------+
  class ResetPeripheral : public HidEngineDetail::Action
  {
  protected:
    void onPress() override;

  public:
  };

  template <uint64_t ID1, uint64_t ID2>
  HidEngineDetail::NotNullActionPtr new_ResetPeripheral()
  {
    static uint8_t buf[sizeof(ResetPeripheral)];
    return new (buf) ResetPeripheral();
  }

}
