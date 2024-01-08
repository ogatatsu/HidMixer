
#include "HidMixer/TinyUsbHidReporterDetail/TinyUsbHidReporter.h"
#include "HidMixer/HidReporterDetail/HidReportDescriptor.h"

using namespace HidMixer::HidReporterDetail;

constexpr TickType_t REPORT_READY_TIMEOUT_MS = 1000;

extern "C"
{
  void tud_hid_report_complete_cb(uint8_t instance, uint8_t const *report, uint16_t len)
  {
    HidMixer::TinyUsbHidReporterDetail::TinyUsbHidReporterClass::report_complete(instance);
  }
}

namespace HidMixer::TinyUsbHidReporterDetail
{
  enum
  {
    REPORT_ID_MOUSE = 1,
    REPORT_ID_CONSUMER_CONTROL,
    REPORT_ID_SYSTEM_CONTROL,
  };

  // clang-format off
  uint8_t const boot_keyboard_hid_report_descriptor[65] = {
    TUD_HID_REPORT_DESC_KEYBOARD()
  };

  uint8_t const other_hid_report_descriptor[137] =
  {
    TUD_HID_REPORT_DESC_MOUSE_EX( HID_REPORT_ID(REPORT_ID_MOUSE) ),
    TUD_HID_REPORT_DESC_CONSUMER( HID_REPORT_ID(REPORT_ID_CONSUMER_CONTROL) ),
    TUD_HID_REPORT_DESC_SYSTEM_CONTROL( HID_REPORT_ID(REPORT_ID_SYSTEM_CONTROL) ),
  };
  // clang-format on

  uint8_t TinyUsbHidReporterClass::_boot_keyboard_instance = 255;
  uint8_t TinyUsbHidReporterClass::_other_hid_instance = 255;
  SemaphoreHandle_t TinyUsbHidReporterClass::_boot_keyboard_report_ready_sem;
  StaticSemaphore_t TinyUsbHidReporterClass::_boot_keyboard_report_ready_sem_buf;
  SemaphoreHandle_t TinyUsbHidReporterClass::_other_hid_report_ready_sem;
  StaticSemaphore_t TinyUsbHidReporterClass::_other_hid_report_ready_sem_buf;

  void TinyUsbHidReporterClass::begin(uint8_t boot_keyboard_instance, uint8_t other_hid_instance)
  {
    _boot_keyboard_instance = boot_keyboard_instance;
    _other_hid_instance = other_hid_instance;

    _boot_keyboard_report_ready_sem = xSemaphoreCreateBinaryStatic(&_boot_keyboard_report_ready_sem_buf);
    xSemaphoreGive(_boot_keyboard_report_ready_sem);
    _other_hid_report_ready_sem = xSemaphoreCreateBinaryStatic(&_other_hid_report_ready_sem_buf);
    xSemaphoreGive(_other_hid_report_ready_sem);
  }

  bool TinyUsbHidReporterClass::wait_report_ready(uint8_t instance)
  {
    if (instance == _boot_keyboard_instance)
    {
      return xSemaphoreTake(_boot_keyboard_report_ready_sem, pdMS_TO_TICKS(REPORT_READY_TIMEOUT_MS)) == pdTRUE;
    }
    else if (instance == _other_hid_instance)
    {
      return xSemaphoreTake(_other_hid_report_ready_sem, pdMS_TO_TICKS(REPORT_READY_TIMEOUT_MS)) == pdTRUE;
    }

    return false;
  }

  bool TinyUsbHidReporterClass::report_complete(uint8_t instance)
  {
    if (instance == _boot_keyboard_instance)
    {
      return xSemaphoreGive(_boot_keyboard_report_ready_sem) == pdTRUE;
    }
    else if (instance == _other_hid_instance)
    {
      return xSemaphoreGive(_other_hid_report_ready_sem) == pdTRUE;
    }

    return false;
  }

  bool TinyUsbHidReporterClass::keyboardReport(uint8_t modifiers, uint8_t key_codes[6])
  {
    if (tud_ready() == false)
    {
      return false;
    }

    hid_keyboard_report_t report = {
        .modifier = modifiers,
        .reserved = 0,
        .keycode = {
            key_codes[0],
            key_codes[1],
            key_codes[2],
            key_codes[3],
            key_codes[4],
            key_codes[5],
        },
    };

    wait_report_ready(_boot_keyboard_instance);
    return tud_hid_n_report(_boot_keyboard_instance, 0, &report, sizeof(hid_keyboard_report_t));
  }

  bool TinyUsbHidReporterClass::mouseReport(uint8_t buttons, int16_t x, int16_t y, int8_t wheel, int8_t pan)
  {
    if (tud_ready() == false)
    {
      return false;
    }

    hid_mouse_report_ex_t report = {
        buttons = buttons,
        .x = x,
        .y = y,
        .wheel = wheel,
        .pan = pan,
    };

    wait_report_ready(_other_hid_instance);
    return tud_hid_n_report(_other_hid_instance, REPORT_ID_MOUSE, &report, sizeof(hid_mouse_report_ex_t));
  }

  bool TinyUsbHidReporterClass::consumerReport(uint16_t usage_code)
  {
    if (tud_ready() == false)
    {
      return false;
    }

    wait_report_ready(_other_hid_instance);
    return tud_hid_n_report(_other_hid_instance, REPORT_ID_CONSUMER_CONTROL, &usage_code, sizeof(usage_code));
  }

  bool TinyUsbHidReporterClass::systemControlReport(uint8_t usage_code)
  {
    if (tud_ready() == false)
    {
      return false;
    }

    wait_report_ready(_other_hid_instance);
    return tud_hid_n_report(_other_hid_instance, REPORT_ID_SYSTEM_CONTROL, &usage_code, sizeof(usage_code));
  }

  bool TinyUsbHidReporterClass::waitReady()
  {
    if (tud_ready() == false)
    {
      return false;
    }

    if (wait_report_ready(_boot_keyboard_instance) == false || wait_report_ready(_other_hid_instance) == false)
    {
      return false;
    }

    report_complete(_boot_keyboard_instance);
    report_complete(_other_hid_instance);
    return true;
  }

  TinyUsbHidReporterClass TinyUsbHidReporter;

}
