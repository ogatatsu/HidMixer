
#include "HidMixer/BluefruitConnectionManagerDetail/services/BLEHid.h"
#include "HidMixer/HidReporterDetail/HidReportDescriptor.h"

using namespace HidMixer::HidReporterDetail;

namespace
{
  enum
  {
    REPORT_ID_KEYBOARD = 1,
    REPORT_ID_MOUSE,
    REPORT_ID_CONSUMER_CONTROL,
    REPORT_ID_SYSTEM_CONTROL,
  };

  // clang-format off
  uint8_t const hid_report_descriptor[204] =
  {
    TUD_HID_REPORT_DESC_KEYBOARD( HID_REPORT_ID(REPORT_ID_KEYBOARD) ),
    TUD_HID_REPORT_DESC_MOUSE_EX( HID_REPORT_ID(REPORT_ID_MOUSE) ),
    TUD_HID_REPORT_DESC_CONSUMER( HID_REPORT_ID(REPORT_ID_CONSUMER_CONTROL) ),
    TUD_HID_REPORT_DESC_SYSTEM_CONTROL( HID_REPORT_ID(REPORT_ID_SYSTEM_CONTROL) ),
  };
  // clang-format on
}

namespace HidMixer::BluefruitConnectionManagerDetail
{

  BLEHid::BLEHid() : BLEHidGeneric(4, 1, 0), _keyboard_led_cb(nullptr)
  {
  }

  err_t BLEHid::begin()
  {
    uint16_t input_len[] = {sizeof(hid_keyboard_report_t), sizeof(hid_mouse_report_ex_t), sizeof(uint16_t), sizeof(uint8_t)};
    uint16_t output_len[] = {1};

    setReportLen(input_len, output_len, NULL);
    enableKeyboard(true);
    enableMouse(true);
    setReportMap(hid_report_descriptor, sizeof(hid_report_descriptor));

    VERIFY_STATUS(BLEHidGeneric::begin());

    return ERROR_NONE;
  }

  void BLEHid::keyboard_output_cb(uint16_t conn_hdl, BLECharacteristic *chr, uint8_t *data, uint16_t len)
  {
    BLEHid &svc = (BLEHid &)chr->parentService();

    if (svc._keyboard_led_cb != nullptr)
    {
      svc._keyboard_led_cb(data[0]);
    }
  }

  void BLEHid::setKeyboardLedCallback(keyboard_led_callback_t cb)
  {
    _keyboard_led_cb = cb;

    // Report mode
    this->setOutputReportCallback(REPORT_ID_KEYBOARD, cb ? keyboard_output_cb : NULL);
    // Boot mode
    _chr_boot_keyboard_output->setWriteCallback(cb ? keyboard_output_cb : NULL);
  }

  bool BLEHid::keyboardReport(uint8_t modifiers, uint8_t key_codes[6])
  {
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

    if (isBootMode())
    {
      return bootKeyboardReport(BLE_CONN_HANDLE_INVALID, &report, sizeof(hid_keyboard_report_t));
    }
    else
    {
      return inputReport(BLE_CONN_HANDLE_INVALID, REPORT_ID_KEYBOARD, &report, sizeof(hid_keyboard_report_t));
    }
  }

  bool BLEHid::mouseReport(uint8_t buttons, int16_t x, int16_t y, int8_t wheel, int8_t pan)
  {
    if (isBootMode())
    {
      hid_mouse_report_t report = {
          buttons = buttons,
          .x = static_cast<int8_t>(constrain(x, -127, 127)),
          .y = static_cast<int8_t>(constrain(y, -127, 127)),
          .wheel = wheel,
          .pan = pan,
      };

      return bootMouseReport(BLE_CONN_HANDLE_INVALID, &report, sizeof(hid_mouse_report_t));
    }
    else
    {
      hid_mouse_report_ex_t report = {
          buttons = buttons,
          .x = x,
          .y = y,
          .wheel = wheel,
          .pan = pan,
      };

      return inputReport(BLE_CONN_HANDLE_INVALID, REPORT_ID_MOUSE, &report, sizeof(hid_mouse_report_ex_t));
    }
  }
  
  bool BLEHid::consumerReport(uint16_t usage_code)
  {
    return inputReport(BLE_CONN_HANDLE_INVALID, REPORT_ID_CONSUMER_CONTROL, &usage_code, sizeof(usage_code));
  }

  bool BLEHid::systemControlReport(uint8_t usage_code)
  {
    return inputReport(BLE_CONN_HANDLE_INVALID, REPORT_ID_SYSTEM_CONTROL, &usage_code, sizeof(usage_code));
  }

  bool BLEHid::waitReady()
  {
    uint8_t conn_hdl = Bluefruit.connHandle();
    BLEConnection *conn = Bluefruit.Connection(conn_hdl);

    if (conn != nullptr)
    {
      if (conn->getHvnPacket())
      {
        conn->releaseHvnPacket();
        return true;
      }
    }
    return false;
  }

}
