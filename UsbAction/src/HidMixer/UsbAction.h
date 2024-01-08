
#pragma once

#include "HidMixer/UsbActionDetail/UsbAction.h"

// RemoteWakeup
#define REMOTE_WAKEUP() (HidMixer::UsbActionDetail::new_RemoteWakeup<__COUNTER__, consthash::city64(__FILE__, sizeof(__FILE__))>())
