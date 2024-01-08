
#include "HidMixer/BluefruitConnectionManagerDetail/clients/BLEClientHHKBStudioHid.h"
#include "bluefruit.h"

namespace HidMixer::BluefruitConnectionManagerDetail
{

  BLEClientHHKBStudioHid::BLEClientHHKBStudioHid(void)
      : BLEClientService(UUID16_SVC_HUMAN_INTERFACE_DEVICE),
        _keyboard_cb(nullptr),
        _trackpoint_cb(nullptr),
        _keyboard_input(UUID16_CHR_REPORT),
        _trackpoint_input(UUID16_CHR_REPORT)
  {
  }

  bool BLEClientHHKBStudioHid::begin()
  {
    // Invoke base class begin()
    BLEClientService::begin();

    _keyboard_input.begin(this);
    _trackpoint_input.begin(this);

    // set notify callback
    _keyboard_input.setNotifyCallback(keyboard_client_notify_cb);
    _trackpoint_input.setNotifyCallback(trackpoint_client_notify_cb);

    return true;
  }

  void BLEClientHHKBStudioHid::setKeyboardReportCallback(keyboard_callback_t fp)
  {
    _keyboard_cb = fp;
  }

  void BLEClientHHKBStudioHid::setTrackpointReportCallback(trackpoint_callback_t fp)
  {
    _trackpoint_cb = fp;
  }

  bool BLEClientHHKBStudioHid::discover(uint16_t conn_handle)
  {
    // Call Base class discover
    VERIFY(BLEClientService::discover(conn_handle));
    _conn_hdl = BLE_CONN_HANDLE_INVALID; // make as invalid until we found all chars

    // Discover all characteristics
    BLEClientCharacteristic *chrs[2] = {
        &_keyboard_input,
        &_trackpoint_input,
    };
    uint8_t cnt = Bluefruit.Discovery.discoverCharacteristic(conn_handle, chrs, 2);

    VERIFY(cnt == 2);

    _conn_hdl = conn_handle;
    return true;
  }

  //------------------------------------------------------------------+
  // Keyboard
  //------------------------------------------------------------------+
  bool BLEClientHHKBStudioHid::enableKeyboard()
  {
    return _keyboard_input.enableNotify();
  }

  bool BLEClientHHKBStudioHid::disableKeyboard()
  {
    return _keyboard_input.disableNotify();
  }

  void BLEClientHHKBStudioHid::_handle_keyboard_input(uint8_t *data, uint16_t len)
  {
    if (_keyboard_cb && (len == sizeof(keyboard_report_t)))
    {
      keyboard_report_t *report = (keyboard_report_t *)data;
      _keyboard_cb(report->modifiers, report->key_codes);
    }
  }

  void BLEClientHHKBStudioHid::keyboard_client_notify_cb(BLEClientCharacteristic *chr, uint8_t *data, uint16_t len)
  {
    BLEClientHHKBStudioHid &svc = (BLEClientHHKBStudioHid &)chr->parentService();
    svc._handle_keyboard_input(data, len);
  }

  //------------------------------------------------------------------+
  // TrackPoint
  //------------------------------------------------------------------+
  bool BLEClientHHKBStudioHid::enableTrackpoint()
  {
    return _trackpoint_input.enableNotify();
  }

  bool BLEClientHHKBStudioHid::disableTrackpoint()
  {
    return _trackpoint_input.disableNotify();
  }

  void BLEClientHHKBStudioHid::_handle_trackpoint_input(uint8_t *data, uint16_t len)
  {
    if (_trackpoint_cb && (len == sizeof(trackpoint_report_t)))
    {
      trackpoint_report_t *report = (trackpoint_report_t *)data;
      _trackpoint_cb(report->buttons, report->x, report->y, report->wheel, report->pan);
    }
  }

  void BLEClientHHKBStudioHid::trackpoint_client_notify_cb(BLEClientCharacteristic *chr, uint8_t *data, uint16_t len)
  {
    BLEClientHHKBStudioHid &svc = (BLEClientHHKBStudioHid &)chr->parentService();
    svc._handle_trackpoint_input(data, len);
  }

}
