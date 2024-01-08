
#pragma once

#include "FreeRTOS.h"
#include "semphr.h"
#include "tusb.h"
#include "HidMixer/HidReporter.h"

namespace HidMixer::TinyUsbHidReporterDetail
{
  extern uint8_t const boot_keyboard_hid_report_descriptor[65];
  extern uint8_t const other_hid_report_descriptor[137];

  class TinyUsbHidReporterClass : public HidReporter
  {
    friend void ::tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report, uint16_t len);

  public:
    static void begin(uint8_t boot_keyboard_instance, uint8_t other_hid_instance);

    bool keyboardReport(uint8_t modifiers, uint8_t key_codes[6]) override;
    bool mouseReport(uint8_t buttons, int16_t x, int16_t y, int8_t wheel, int8_t pan) override;
    bool consumerReport(uint16_t usage_code) override;
    bool systemControlReport(uint8_t usage_code) override;
    bool waitReady() override;

  private:
    static bool wait_report_ready(uint8_t instance);
    static bool report_complete(uint8_t instance);

    static uint8_t _boot_keyboard_instance;
    static uint8_t _other_hid_instance;
    static SemaphoreHandle_t _boot_keyboard_report_ready_sem;
    static StaticSemaphore_t _boot_keyboard_report_ready_sem_buf;
    static SemaphoreHandle_t _other_hid_report_ready_sem;
    static StaticSemaphore_t _other_hid_report_ready_sem_buf;
  };

  extern TinyUsbHidReporterClass TinyUsbHidReporter;

}
