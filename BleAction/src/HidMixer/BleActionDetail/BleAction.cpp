
#include "HidMixer/BleActionDetail/BleAction.h"
#include "HidMixer/BluefruitConnectionManager.h"

namespace HidMixer::BleActionDetail
{
  //------------------------------------------------------------------+
  // ResetCentral
  //------------------------------------------------------------------+
  void ResetCentral::onPress()
  {
    HidMixer::BluefruitConnectionManager.Central.clearBond();
  }

  //------------------------------------------------------------------+
  // ResetPeripheral
  //------------------------------------------------------------------+
  void ResetPeripheral::onPress()
  {
    HidMixer::BluefruitConnectionManager.Periph.clearBond();
  }

}
