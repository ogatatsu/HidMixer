
#include "HidMixer/HidEngineDetail/HidCore.h"
#include "HidMixer/HidEngineDetail/HidEngine_config.h"
#include "HidMixer/Set.h"
#include "task.h"
#include <string.h>

#define KEY_REPORT_MIN_INTERVAL_TICKS (pdMS_TO_TICKS(HID_ENGINE_KEY_REPORT_MIN_INTERVAL_MS))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)

#define bitSet(value, bit) ((value) |= (1UL << (bit)))

#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))

namespace HidMixer::HidEngineDetail
{
  HidReporter *HidCore::_hid_reporter = nullptr;
  uint8_t HidCore::_pressed_keys[7] = {};
  uint8_t HidCore::_prev_sent_keys[6] = {};
  uint8_t HidCore::_key_counters[256] = {};
  uint8_t HidCore::_pressed_modifiers = 0;
  uint8_t HidCore::_prev_sent_modifiers = 0;
  uint8_t HidCore::_modifier_counters[8] = {};
  uint8_t HidCore::_pressed_pointing_buttons = 0;
  uint8_t HidCore::_prev_sent_pointing_buttons = 0;
  uint8_t HidCore::_pointing_button_counters[5] = {};
  TickType_t HidCore::_last_send_ticks = 0;

  void HidCore::setReporter(HidReporter *hid_reporter)
  {
    _hid_reporter = hid_reporter;
  }

  void HidCore::waitReady()
  {
    if (_hid_reporter != nullptr)
    {
      _hid_reporter->waitReady();
    }
  }

  //------------------------------------------------------------------+
  // Keyboard API
  //------------------------------------------------------------------+

  void HidCore::keyPress(CharacterKeyCode character_key)
  {
    setKey(character_key);
    sendKeyReport();
  }

  void HidCore::keyRelease(CharacterKeyCode character_key)
  {
    unsetKey(character_key);
    sendKeyReport();
  }

  void HidCore::modifiersPress(ModifiersCode modifiers)
  {
    setModifiers(modifiers);
    sendKeyReport();
  }

  void HidCore::modifiersRelease(ModifiersCode modifiers)
  {
    unsetModifiers(modifiers);
    sendKeyReport();
  }

  void HidCore::setKey(CharacterKeyCode character_key)
  {
    uint8_t code = static_cast<uint8_t>(character_key);
    _key_counters[code]++;

    for (size_t i = 0; i < 6; i++)
    {
      // すでに入ってるなら追加しない
      if (_pressed_keys[i] == code)
      {
        return;
      }

      // 開いているスペースを探して追加する。
      // 0が見つかった時点でそれ以降はすべて0、すでに入っているかを気にする必要はない
      if (_pressed_keys[i] == 0)
      {
        _pressed_keys[i] = code;
        return;
      }
    }

    // 満杯なら最初を削除、スライドして末尾に追加
    memmove(_pressed_keys, _pressed_keys + 1, 5);
    _pressed_keys[5] = code;
  }

  void HidCore::unsetKey(CharacterKeyCode character_key)
  {
    uint8_t code = static_cast<uint8_t>(character_key);
    _key_counters[code]--;

    // まだ押されてるなら削除しない
    if (_key_counters[code] > 0)
    {
      return;
    }

    size_t i = 0;
    // 一致するインデックスを検索
    for (; i < 6; i++)
    {
      if (_pressed_keys[i] == code)
      {
        break;
      }
    }
    // 見つかったらスライドして削除
    // _pressed_keys[6]は常に0が入っているので末尾には0が補充される
    for (; i < 6; i++)
    {
      _pressed_keys[i] = _pressed_keys[i + 1];
    }
  }

  void HidCore::setModifiers(ModifiersCode modifiers)
  {
    for (size_t i = 0; i < 8; i++)
    {
      if (bitRead(static_cast<uint8_t>(modifiers), i))
      {
        _modifier_counters[i]++;
        bitSet(_pressed_modifiers, i);
      }
    }
  }

  void HidCore::unsetModifiers(ModifiersCode modifiers)
  {
    for (size_t i = 0; i < 8; i++)
    {
      if (bitRead(static_cast<uint8_t>(modifiers), i))
      {
        _modifier_counters[i]--;
        if (_modifier_counters[i] <= 0)
        {
          bitClear(_pressed_modifiers, i);
        }
      }
    }
  }

  bool HidCore::isModifiersSet()
  {
    return _pressed_modifiers != 0;
  }

  void HidCore::sendKeyReport()
  {
    // 前回送ったreportと比較して変更があるか
    bool is_key_changed = false;
    bool is_mod_changed = false;

    // 前回の6keyとの比較して変更があるか計算する
    if (memcmp(_prev_sent_keys, _pressed_keys, sizeof(_prev_sent_keys)) != 0)
    {
      is_key_changed = true;
      // 次回用に保存
      memcpy(_prev_sent_keys, _pressed_keys, sizeof(_prev_sent_keys));
    }

    // 前回のmodifierとの比較
    if (_pressed_modifiers != _prev_sent_modifiers)
    {
      is_mod_changed = true;
      // 次回用に保存
      _prev_sent_modifiers = _pressed_modifiers;
    }

    // 変更が無いなら終了
    if (is_key_changed == false && is_mod_changed == false)
    {
      return;
    }

    if (_hid_reporter != nullptr)
    {
      vTaskDelayUntil(&_last_send_ticks, KEY_REPORT_MIN_INTERVAL_TICKS);
      _hid_reporter->keyboardReport(_pressed_modifiers, _pressed_keys);
      _last_send_ticks = xTaskGetTickCount();
    }
  }

  //------------------------------------------------------------------+
  // Consumer Control API
  //------------------------------------------------------------------+

  void HidCore::consumerControlPress(ConsumerControlCode usage_code)
  {
    if (_hid_reporter != nullptr)
    {
      _hid_reporter->consumerReport(static_cast<uint16_t>(usage_code));
    }
  }

  void HidCore::consumerControlRelease()
  {
    if (_hid_reporter != nullptr)
    {
      _hid_reporter->consumerReport(0);
    }
  }

  //------------------------------------------------------------------+
  // System Control API
  //------------------------------------------------------------------+

  void HidCore::systemControlPress(SystemControlCode usage_code)
  {
    if (_hid_reporter != nullptr)
    {
      _hid_reporter->systemControlReport(static_cast<uint8_t>(usage_code));
    }
  }

  void HidCore::systemControlRelease()
  {
    if (_hid_reporter != nullptr)
    {
      _hid_reporter->systemControlReport(0);
    }
  }

  //------------------------------------------------------------------+
  // Poinding Device API
  //------------------------------------------------------------------+

  void HidCore::pointerMove(int16_t x, int16_t y)
  {
    if (_hid_reporter != nullptr)
    {
      _hid_reporter->mouseReport(_prev_sent_pointing_buttons, x, y, 0, 0);
    }
  }

  void HidCore::pointingScroll(int8_t scroll, int8_t pan)
  {
    if (_hid_reporter != nullptr)
    {
      vTaskDelayUntil(&_last_send_ticks, KEY_REPORT_MIN_INTERVAL_TICKS);
      _hid_reporter->mouseReport(_prev_sent_pointing_buttons, 0, 0, scroll, pan);
      _last_send_ticks = xTaskGetTickCount();
    }
  }

  void HidCore::pointingButtonsPress(PointingButtonsCode buttons)
  {
    setPointingButtons(buttons);
    sendPointingButtonsReport();
  }

  void HidCore::pointingButtonsRelease(PointingButtonsCode buttons)
  {
    unsetPointingButtons(buttons);
    sendPointingButtonsReport();
  }

  void HidCore::setPointingButtons(PointingButtonsCode buttons)
  {
    for (size_t i = 0; i < 5; i++)
    {
      if (bitRead(static_cast<uint8_t>(buttons), i))
      {
        _pointing_button_counters[i]++;
        bitSet(_pressed_pointing_buttons, i);
      }
    }
  }

  void HidCore::unsetPointingButtons(PointingButtonsCode buttons)
  {
    for (size_t i = 0; i < 5; i++)
    {
      if (bitRead(static_cast<uint8_t>(buttons), i))
      {
        _pointing_button_counters[i]--;
        if (_pointing_button_counters[i] <= 0)
        {
          bitClear(_pressed_pointing_buttons, i);
        }
      }
    }
  }

  void HidCore::sendPointingButtonsReport()
  {
    if (_pressed_pointing_buttons == _prev_sent_pointing_buttons)
    {
      return;
    }

    if (_hid_reporter != nullptr)
    {
      vTaskDelayUntil(&_last_send_ticks, KEY_REPORT_MIN_INTERVAL_TICKS);
      _hid_reporter->mouseReport(_pressed_pointing_buttons, 0, 0, 0, 0);
      _last_send_ticks = xTaskGetTickCount();
    }
    _prev_sent_pointing_buttons = _pressed_pointing_buttons;
  }

  HidCore Hid;

}
