
#include "BLEClientRelaconHid.h"
#include "bluefruit.h"

namespace HidMixer::BluefruitConnectionManagerDetail
{

  BLEClientRelaconHid::BLEClientRelaconHid(void)
      : BLEClientService(UUID16_SVC_HUMAN_INTERFACE_DEVICE),
        _trackball_cb(nullptr),
        _consumer_cb(nullptr),
        _trackball_input(UUID16_CHR_REPORT),
        _consumer_input(UUID16_CHR_REPORT)
  {
  }

  bool BLEClientRelaconHid::begin()
  {
    // Invoke base class begin()
    BLEClientService::begin();

    _trackball_input.begin(this);
    _consumer_input.begin(this);

    // set notify callback
    _trackball_input.setNotifyCallback(trackball_client_notify_cb);
    _consumer_input.setNotifyCallback(consumer_client_notify_cb);

    return true;
  }

  void BLEClientRelaconHid::setTrackballReportCallback(trackball_callback_t fp)
  {
    _trackball_cb = fp;
  }

  void BLEClientRelaconHid::setConsumerReportCallback(consumer_callback_t fp)
  {
    _consumer_cb = fp;
  }

  bool BLEClientRelaconHid::discover(uint16_t conn_handle)
  {
    // Call Base class discover
    VERIFY(BLEClientService::discover(conn_handle));
    _conn_hdl = BLE_CONN_HANDLE_INVALID; // make as invalid until we found all chars

    // Discover all characteristics
    Bluefruit.Discovery.discoverCharacteristic(conn_handle, _trackball_input, _consumer_input);

    VERIFY(_trackball_input.discovered() && _consumer_input.discovered());

    _conn_hdl = conn_handle;
    return true;
  }

  //------------------------------------------------------------------+
  // Trackball
  //------------------------------------------------------------------+
  bool BLEClientRelaconHid::enableTrackball()
  {
    return _trackball_input.enableNotify();
  }

  bool BLEClientRelaconHid::disableTrackball()
  {
    return _trackball_input.disableNotify();
  }

  void BLEClientRelaconHid::_handle_trackball_input(uint8_t *data, uint16_t len)
  {
    if (_trackball_cb && (len == sizeof(trackball_report_t)))
    {
      trackball_report_t *report = (trackball_report_t *)data;
      _trackball_cb(report->buttons, report->x, report->y, report->wheel);
    }
  }

  void BLEClientRelaconHid::trackball_client_notify_cb(BLEClientCharacteristic *chr, uint8_t *data, uint16_t len)
  {
    BLEClientRelaconHid &svc = (BLEClientRelaconHid &)chr->parentService();
    svc._handle_trackball_input(data, len);
  }

  //------------------------------------------------------------------+
  // Consumer
  //------------------------------------------------------------------+
  bool BLEClientRelaconHid::enableConsumer()
  {
    return _consumer_input.enableNotify();
  }

  bool BLEClientRelaconHid::disableConsumer()
  {
    return _consumer_input.disableNotify();
  }

  void BLEClientRelaconHid::_handle_consumer_input(uint8_t *data, uint16_t len)
  {
    if (_consumer_cb && (len == sizeof(consumer_report_t)))
    {
      consumer_report_t *report = (consumer_report_t *)data;
      _consumer_cb(report->usage_code);
    }
  }

  void BLEClientRelaconHid::consumer_client_notify_cb(BLEClientCharacteristic *chr, uint8_t *data, uint16_t len)
  {
    BLEClientRelaconHid &svc = (BLEClientRelaconHid &)chr->parentService();
    svc._handle_consumer_input(data, len);
  }

}
