
#include "HidMixer/UsbActionDetail/UsbAction.h"
#include "tusb.h"

namespace HidMixer::UsbActionDetail
{

  //------------------------------------------------------------------+
  // RemoteWakeup
  //------------------------------------------------------------------+
  void RemoteWakeup::onPress()
  {
    if (tud_suspended())
    {
      tud_remote_wakeup();
    }
  }

}
