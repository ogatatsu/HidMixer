
#include "HidMixer/BluefruitConnectionManagerDetail/services/BLEUartNonArduinoStream.h"
#include "bluefruit.h"

namespace HidMixer::BluefruitConnectionManagerDetail
{

  // Constructor
  BLEUartNonArduinoStream::BLEUartNonArduinoStream()
      : BLEService(BLEUART_UUID_SERVICE),
        _txd(BLEUART_UUID_CHR_TXD),
        _rxd(BLEUART_UUID_CHR_RXD),
        _rx_cb(nullptr),
        _notify_cb(nullptr)
  {
  }

  // Callback when received new data
  void BLEUartNonArduinoStream::bleuart_rxd_cb(uint16_t conn_hdl, BLECharacteristic *chr, uint8_t *data, uint16_t len)
  {
    BLEUartNonArduinoStream &svc = (BLEUartNonArduinoStream &)chr->parentService();

    // invoke user callback
    if (svc._rx_cb)
    {
      svc._rx_cb(data, len);
    }
  }

  void BLEUartNonArduinoStream::bleuart_txd_cccd_cb(uint16_t conn_hdl, BLECharacteristic *chr, uint16_t value)
  {
    BLEUartNonArduinoStream &svc = (BLEUartNonArduinoStream &)chr->parentService();

    if (svc._notify_cb)
    {
      svc._notify_cb(value & BLE_GATT_HVX_NOTIFICATION);
    }
  }

  void BLEUartNonArduinoStream::setRxCallback(rx_callback_t fp)
  {
    _rx_cb = fp;
  }

  void BLEUartNonArduinoStream::setNotifyCallback(notify_callback_t fp)
  {
    _notify_cb = fp;
    _txd.setCccdWriteCallback(fp ? BLEUartNonArduinoStream::bleuart_txd_cccd_cb : NULL);
  }

  err_t BLEUartNonArduinoStream::begin(void)
  {
    // Invoke base class begin()
    VERIFY_STATUS(BLEService::begin());

    uint16_t max_mtu = Bluefruit.getMaxMtu(BLE_GAP_ROLE_PERIPH);

    // Add TXD Characteristic
    _txd.setProperties(CHR_PROPS_NOTIFY);
    _txd.setPermission(SECMODE_ENC_NO_MITM, SECMODE_NO_ACCESS);
    _txd.setMaxLen(max_mtu);
    _txd.setUserDescriptor("TXD");
    VERIFY_STATUS(_txd.begin());

    // Add RXD Characteristic
    _rxd.setProperties(CHR_PROPS_WRITE | CHR_PROPS_WRITE_WO_RESP);
    _rxd.setWriteCallback(BLEUartNonArduinoStream::bleuart_rxd_cb);
    _rxd.setPermission(SECMODE_NO_ACCESS, SECMODE_ENC_NO_MITM);
    _rxd.setMaxLen(max_mtu);
    _rxd.setUserDescriptor("RXD");
    VERIFY_STATUS(_rxd.begin());

    return ERROR_NONE;
  }

  bool BLEUartNonArduinoStream::notifyEnabled(void)
  {
    return _txd.notifyEnabled(Bluefruit.connHandle());
  }

  uint16_t BLEUartNonArduinoStream::write(const uint8_t *content, uint16_t len)
  {
    uint16_t conn_hdl = Bluefruit.connHandle();
    BLEConnection *conn = Bluefruit.Connection(Bluefruit.connHandle());
    VERIFY(conn, 0);

    // skip if not enabled
    if (!notifyEnabled())
    {
      return 0;
    }

    uint16_t max_payload = conn->getMtu() - 3;
    len = min(max_payload, len);
    return _txd.notify(conn_hdl, content, len) ? len : 0;
  }

}
