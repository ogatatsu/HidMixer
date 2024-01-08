#pragma once

#include "HidMixer/BleActionDetail/BleAction.h"

// ResetCentral
#define RESET_CENTRAL() (HidMixer::BleActionDetail::new_ResetCentral<__COUNTER__, consthash::city64(__FILE__, sizeof(__FILE__))>())

// ResetPeripheral
#define RESET_PERIPHERAL() (HidMixer::BleActionDetail::new_ResetPeripheral<__COUNTER__, consthash::city64(__FILE__, sizeof(__FILE__))>())
