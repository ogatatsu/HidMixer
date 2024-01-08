
#include "HidMixer/HidEngineDetail/ActionTapper.h"
#include "HidMixer/HidEngineDetail/HidCore.h"
#include "HidMixer/HidEngineDetail/HidEngine.h"
#include "HidMixer/HidEngineDetail/HidEngineTask.h"
#include "HidMixer/HidEngineDetail/utility.h"

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)

namespace HidMixer::HidEngineDetail
{

  etl::span<Key> HidEngineClass::_keymap;
  etl::span<KeyShift> HidEngineClass::_key_shift_map;
  etl::span<Combo> HidEngineClass::_combo_map;
  etl::span<Gesture> HidEngineClass::_gesture_map;
  etl::span<Encoder> HidEngineClass::_encoder_map;
  etl::span<EncoderShift> HidEngineClass::_encoder_shift_map;

  HidEngineClass::read_pointer_delta_callback_t HidEngineClass::_read_pointer_delta_cb = nullptr;
  HidEngineClass::read_encoder_step_callback_t HidEngineClass::_read_encoder_step_cb = nullptr;

  HidEngineClass::ComboInterruptionEvent HidEngineClass::_combo_interruption_event;

  etl::intrusive_list<Key, PressedKeyLink> HidEngineClass::_pressed_key_list;
  etl::intrusive_list<KeyShiftIdLink, StartedKeyShiftIdLink> HidEngineClass::_started_key_shift_id_list;
  etl::intrusive_list<GestureIdLink, StartedGestureIdLink> HidEngineClass::_started_gesture_id_list;
  etl::intrusive_list<EncoderShiftIdLink, StartedEncoderShiftIdLink> HidEngineClass::_started_encoder_shift_id_list;

  void HidEngineClass::setKeymap(etl::span<Key> keymap)
  {
    _keymap = keymap;
  }

  void HidEngineClass::setKeyShiftMap(etl::span<KeyShift> key_shift_map)
  {
    _key_shift_map = key_shift_map;
  }

  void HidEngineClass::setComboMap(etl::span<Combo> combo_map)
  {
    _combo_map = combo_map;
  }

  void HidEngineClass::setGestureMap(etl::span<Gesture> gesture_map)
  {
    _gesture_map = gesture_map;
  }

  void HidEngineClass::setEncoderMap(etl::span<Encoder> encoder_map)
  {
    _encoder_map = encoder_map;
  }

  void HidEngineClass::setEncoderShiftMap(etl::span<EncoderShift> encoder_shift_map)
  {
    _encoder_shift_map = encoder_shift_map;
  }

  void HidEngineClass::setHidReporter(HidReporter *hid_reporter)
  {
    Hid.setReporter(hid_reporter);
  }

#ifdef configUSE_CORE_AFFINITY
  void HidEngineClass::start(UBaseType_t task_priority, UBaseType_t core_affinity_mask)
  {
    initAction();
    HidEngineTask.start(task_priority, core_affinity_mask);
  }
#else
  void HidEngineClass::start(UBaseType_t task_priority)
  {
    initAction();
    HidEngineTask.start(task_priority);
  }
#endif

  void HidEngineClass::initAction()
  {
    TimerMixin::init();
    ActionTapper.init();

    for (auto &key : _keymap)
    {
      key.action->init(key.key_id);
    }

    for (auto &key_shift : _key_shift_map)
    {
      for (auto &key : key_shift.keymap)
      {
        key.action->init(key.key_id);
      }
    }

    for (auto &combo : _combo_map)
    {
      combo.action->init(etl::nullopt);
    }

    for (auto &gesture : _gesture_map)
    {
      gesture.up_action->init(etl::nullopt);
      gesture.down_action->init(etl::nullopt);
      gesture.left_action->init(etl::nullopt);
      gesture.right_action->init(etl::nullopt);
    }

    for (auto &encoder : _encoder_map)
    {
      encoder.clockwise_action->init(etl::nullopt);
      encoder.counterclockwise_action->init(etl::nullopt);
    }

    for (auto &encoder_shift : _encoder_shift_map)
    {
      encoder_shift.clockwise_action->init(etl::nullopt);
      encoder_shift.counterclockwise_action->init(etl::nullopt);
    }
  }

  void HidEngineClass::applyToKeymap(const Set &key_ids)
  {
    EventData evt{ApplyToKeymapEventData{key_ids}};
    HidEngineTask.enqueEvent(evt);
  }

  void HidEngineClass::movePointer(PointingDeviceId pointing_device_id)
  {
    EventData evt{MovePointerEventData{pointing_device_id}};
    HidEngineTask.enqueEvent(evt);
  }

  void HidEngineClass::rotateEncoder(EncoderId encoder_id)
  {
    EventData evt{RotateEncoderEventData{encoder_id}};
    HidEngineTask.enqueEvent(evt);
  }

  void HidEngineClass::setReadPointerDeltaCallback(read_pointer_delta_callback_t cb)
  {
    _read_pointer_delta_cb = cb;
  }

  void HidEngineClass::setReadEncoderStepCallback(read_encoder_step_callback_t cb)
  {
    _read_encoder_step_cb = cb;
  }

  //------------------------------------------------------------------+
  // ApplyToKeymap
  //------------------------------------------------------------------+
  void HidEngineClass::applyToKeymap_body(Set &key_ids)
  {
    static Set prev_ids;

    {
      Set press_ids = key_ids - prev_ids;

      uint8_t arr[press_ids.count()];
      press_ids.toArray(arr);

      for (uint8_t key_id : arr)
      {
        processComboAndKey(Event::Press, key_id);
      }
    }

    {
      Set release_ids = prev_ids - key_ids;

      uint8_t arr[release_ids.count()];
      release_ids.toArray(arr);

      for (uint8_t key_id : arr)
      {
        processComboAndKey(Event::Release, key_id);
      }
    }

    prev_ids = key_ids;
  }

  void HidEngineClass::processComboAndKey(Event event, etl::optional<uint8_t> key_id)
  {
    static etl::optional<uint8_t> first_combo_id;
    static etl::intrusive_list<Combo, SuccessComboLink> success_combo_list;

    switch (event)
    {
    case Event::Press:
    {
      // コンボ実行中のid（片方のキーだけreleaseされて再度pressされた。）の場合は新しいコンボを開始しないで通常のKeyPress
      for (auto &combo : success_combo_list)
      {
        if (combo.first_id == key_id || combo.second_id == key_id)
        {
          performKeyPress(key_id.value());
          return;
        }
      }

      // first_id check
      if (first_combo_id.has_value() == false)
      {
        for (auto &combo : _combo_map)
        {
          if (combo.isMatchFirstId(key_id.value()))
          {
            // first_id success
            first_combo_id = key_id;
            _combo_interruption_event.start(combo.combo_term_ms);
            return;
          }
        }
        // first_id failure
        performKeyPress(key_id.value());
        return;
      }

      // second_id check
      for (auto &combo : _combo_map)
      {
        if (combo.isMatchIds(first_combo_id.value(), key_id.value()))
        {
          // combo success
          _combo_interruption_event.stop();
          combo.first_id_released = false;
          combo.second_id_released = false;
          combo.action->press();
          success_combo_list.push_back(combo);
          first_combo_id = etl::nullopt;
          return;
        }
      }

      // second_id failure
      _combo_interruption_event.stop();
      performKeyPress(first_combo_id.value());
      performKeyPress(key_id.value());
      first_combo_id = etl::nullopt;
    }
    break;

    case Event::Release:
    {
      // combo実行中のidがreleaseされた場合
      // for (auto &combo : success_combo_list)
      for (auto combo = success_combo_list.begin(); combo != success_combo_list.end(); combo++)
      {
        if (combo->first_id == key_id && combo->first_id_released == false)
        {
          combo->first_id_released = true;

          if ((combo->isFastRelease() && combo->second_id_released == false) ||
              (combo->isSlowRelease() && combo->second_id_released))
          {
            combo->action->release();
          }

          if (combo->second_id_released)
          {
            success_combo_list.erase(combo);
          }
          return;
        }

        if (combo->second_id == key_id && combo->second_id_released == false)
        {
          combo->second_id_released = true;

          if ((combo->isFastRelease() && combo->first_id_released == false) ||
              (combo->isSlowRelease() && combo->first_id_released))
          {
            combo->action->release();
          }

          if (combo->first_id_released)
          {
            success_combo_list.erase(combo);
          }
          return;
        }
      }

      // first_idがタップされた場合
      if (first_combo_id == key_id)
      {
        _combo_interruption_event.stop();
        first_combo_id = etl::nullopt;
        performKeyPress(key_id.value());
        performKeyRelease(key_id.value());
        return;
      }

      // first_idより前に押されていたidがreleaseされた場合
      if (first_combo_id.has_value())
      {
        _combo_interruption_event.stop();
        performKeyPress(first_combo_id.value());
        performKeyRelease(key_id.value());
        first_combo_id = etl::nullopt;
        return;
      }

      // 他
      performKeyRelease(key_id.value());
    }
    break;

    case Event::ComboInterruption:
    {
      _combo_interruption_event.stop();
      if (first_combo_id.has_value())
      {
        performKeyPress(first_combo_id.value());
        first_combo_id = etl::nullopt;
      }
    }
    break;

    default:
      break;
    }
  }

  void HidEngineClass::performKeyPress(uint8_t key_id)
  {
    for (auto &key : _pressed_key_list)
    {
      if (key.key_id == key_id)
      {
        return;
      }
    }

    BeforeOtherKeyPressEventListener::_notifyBeforeOtherKeyPress(key_id);

    auto tpl = getCurrentKey(key_id);
    auto key_shift = std::get<0>(tpl);
    auto key = std::get<1>(tpl);

    if (key == nullptr)
    {
      return;
    }

    if (key_shift != nullptr &&
        key_shift->pre_action.has_value() &&
        key_shift->pre_action.value().is_pressed == false)
    {
      key_shift->pre_action.value().is_pressed = true;
      key_shift->pre_action.value().action->press();

      if (key_shift->pre_action.value().timing == Timing::InsteadOfFirstAction)
      {
        return;
      }
    }

    _pressed_key_list.push_front(*key);
    key->action->press();
  }

  void HidEngineClass::performKeyRelease(uint8_t key_id)
  {
    for (auto itr = _pressed_key_list.begin(); itr != _pressed_key_list.end(); itr++)
    {
      if (itr->key_id == key_id)
      {
        itr->action->release();
        _pressed_key_list.erase(itr);
        return;
      }
    }
  }

  std::tuple<KeyShift *, Key *> HidEngineClass::getCurrentKey(uint8_t key_id)
  {
    for (auto &started_key_shift_id : _started_key_shift_id_list)
    {
      for (auto &key_shift : _key_shift_map)
      {
        if (key_shift.key_shift_id.value == started_key_shift_id.value)
        {
          for (auto &key : key_shift.keymap)
          {
            if (key.key_id == key_id)
            {
              return {&key_shift, &key};
            }
          }
          if (key_shift.keymap_overlay == KeymapOverlay::Disable)
          {
            return {&key_shift, nullptr};
          }
        }
      }
    }

    for (auto &key : _keymap)
    {
      if (key.key_id == key_id)
      {
        return {nullptr, &key};
      }
    }

    return {nullptr, nullptr};
  }

  void HidEngineClass::startKeyShift(KeyShiftIdLink &key_shift_id_link)
  {
    if (key_shift_id_link.is_linked())
    {
      return;
    }

    _started_key_shift_id_list.push_front(key_shift_id_link);

    // pre_action
    for (auto &key_shift : _key_shift_map)
    {
      if (key_shift.key_shift_id.value == key_shift_id_link.value &&
          key_shift.pre_action.has_value() &&
          key_shift.pre_action.value().timing == Timing::Immediately &&
          key_shift.pre_action.value().is_pressed == false)
      {
        key_shift.pre_action.value().is_pressed = true;
        key_shift.pre_action.value().action->press();
      }
    }
  }

  void HidEngineClass::stopKeyShift(KeyShiftIdLink &key_shift_id_link)
  {
    if (key_shift_id_link.is_linked() == false)
    {
      return;
    }

    erase_link_from_list(_started_key_shift_id_list, key_shift_id_link);

    // 同じidが同時に押されることもあり得るので最後に押されていたかチェック
    for (auto &started_key_shift_id : _started_key_shift_id_list)
    {
      if (started_key_shift_id.value == key_shift_id_link.value)
      {
        return;
      }
    }

    // 最後のidならclean up
    for (auto &key_shift : _key_shift_map)
    {
      if (key_shift.key_shift_id.value == key_shift_id_link.value &&
          key_shift.pre_action.has_value() &&
          key_shift.pre_action.value().is_pressed == true)
      {
        key_shift.pre_action.value().is_pressed = false;
        key_shift.pre_action.value().action->release();
      }
    }
  }

  //------------------------------------------------------------------+
  // MovePointer
  //------------------------------------------------------------------+
  void HidEngineClass::movePointer_body(PointingDeviceId pointing_device_id)
  {
    if (_read_pointer_delta_cb == nullptr)
    {
      return;
    }

    Hid.waitReady();
    int16_t delta_x = 0, delta_y = 0;
    _read_pointer_delta_cb(pointing_device_id, delta_x, delta_y);

    if (delta_x == 0 && delta_y == 0)
    {
      return;
    }

    BeforePointerMoveEventListener::_notifyBeforePointerMove(pointing_device_id, delta_x, delta_y);

    Gesture *gesture = getCurrentGesture(pointing_device_id);

    if (gesture != nullptr)
    {
      processGesture(*gesture, delta_x, delta_y);
    }
    else
    {
      Hid.pointerMove(delta_x, delta_y);
    }
  }

  Gesture *HidEngineClass::getCurrentGesture(PointingDeviceId pointing_device_id)
  {
    for (auto &started_gesture_id : _started_gesture_id_list)
    {
      for (auto &gesture : _gesture_map)
      {
        if ((started_gesture_id.value == gesture.gesture_id.value) && (gesture.pointing_device_id == pointing_device_id))
        {
          return &gesture;
        }
      }
    }

    return nullptr;
  }

  void HidEngineClass::processGesture(Gesture &gesture, int16_t delta_x, int16_t delta_y)
  {
#if (HID_ENGINE_INSTEAD_OF_FIRST_GESTURE_AFTER_WAIT_TIME_MS != 0)
    if (gesture.instead_of_first_gesture_millis.has_value())
    {
      uint32_t curr_millis = millis();
      if (static_cast<uint32_t>(curr_millis - gesture.instead_of_first_gesture_millis.value()) <= HID_ENGINE_INSTEAD_OF_FIRST_GESTURE_AFTER_WAIT_TIME_MS)
      {
        return;
      }

      gesture.instead_of_first_gesture_millis = etl::nullopt;
    }
#endif

    // 逆方向に動いたら距離をリセット
    if (bitRead(gesture.total_distance_x ^ static_cast<int32_t>(delta_x), 31))
    {
      gesture.total_distance_x = 0;
    }
    if (bitRead(gesture.total_distance_y ^ static_cast<int32_t>(delta_y), 31))
    {
      gesture.total_distance_y = 0;
    }

    // 距離を足す
    gesture.total_distance_x += delta_x;
    gesture.total_distance_y += delta_y;

    // 直近のマウスの移動距離の大きさによって実行する順序を変える
    if (abs(delta_x) >= abs(delta_y))
    {
      processGestureX(gesture);
      processGestureY(gesture);
    }
    else
    {
      processGestureY(gesture);
      processGestureX(gesture);
    }
  }

  void HidEngineClass::processGestureX(Gesture &gesture)
  {
    uint8_t n_times = std::min<int32_t>(abs(gesture.total_distance_x / gesture.distance), UINT8_MAX);

    if (n_times == 0)
    {
      return;
    }

    BeforeGestureEventListener::_notifyBeforeGesture(gesture.gesture_id, gesture.pointing_device_id);

    bool is_right = gesture.total_distance_x > 0;
    gesture.total_distance_x %= gesture.distance;

    if (gesture.angle_snap == AngleSnap::Enable)
    {
      gesture.total_distance_y = 0;
    }

    processGesturePreAction(gesture, n_times);

    if (n_times == 0)
    {
      return;
    }

    if (is_right)
    {
      ActionTapper.tap(gesture.right_action, n_times);
    }
    else
    {
      ActionTapper.tap(gesture.left_action, n_times);
    }
  }

  void HidEngineClass::processGestureY(Gesture &gesture)
  {
    uint8_t n_times = std::min<int32_t>(abs(gesture.total_distance_y / gesture.distance), UINT8_MAX);

    if (n_times == 0)
    {
      return;
    }

    BeforeGestureEventListener::_notifyBeforeGesture(gesture.gesture_id, gesture.pointing_device_id);

    bool is_down = gesture.total_distance_y > 0;
    gesture.total_distance_y %= gesture.distance;

    if (gesture.angle_snap == AngleSnap::Enable)
    {
      gesture.total_distance_x = 0;
    }

    processGesturePreAction(gesture, n_times);

    if (n_times == 0)
    {
      return;
    }

    if (is_down)
    {
      ActionTapper.tap(gesture.down_action, n_times);
    }
    else
    {
      ActionTapper.tap(gesture.up_action, n_times);
    }
  }

  void HidEngineClass::processGesturePreAction(Gesture &gesture, uint8_t &n_times)
  {
    if (gesture.pre_action.has_value() &&
        gesture.pre_action.value().is_pressed == false)
    {
      gesture.pre_action.value().is_pressed = true;
      gesture.pre_action.value().action->press();

      if (gesture.pre_action.value().timing == Timing::InsteadOfFirstAction)
      {
#if (HID_ENGINE_INSTEAD_OF_FIRST_GESTURE_AFTER_WAIT_TIME_MS != 0)
        gesture.instead_of_first_gesture_millis = millis();
        n_times = 0;
        gesture.total_distance_x = 0;
        gesture.total_distance_y = 0;
#else
        n_times--;
#endif
      }
    }
  }

  void HidEngineClass::startGesture(GestureIdLink &gesture_id_link)
  {
    if (gesture_id_link.is_linked())
    {
      return;
    }

    _started_gesture_id_list.push_front(gesture_id_link);

    // pre_action
    for (auto &gesture : _gesture_map)
    {
      if (gesture.gesture_id.value == gesture_id_link.value &&
          gesture.pre_action.has_value() &&
          gesture.pre_action.value().timing == Timing::Immediately &&
          gesture.pre_action.value().is_pressed == false)
      {
        gesture.pre_action.value().is_pressed = true;
        gesture.pre_action.value().action->press();
      }
    }
  }

  void HidEngineClass::stopGesture(GestureIdLink &gesture_id_link)
  {
    if (gesture_id_link.is_linked() == false)
    {
      return;
    }

    erase_link_from_list(_started_gesture_id_list, gesture_id_link);

    // 同じidが同時に押されることもあり得るので最後に押されていたかチェック
    for (auto &started_gesture_id : _started_gesture_id_list)
    {
      if (started_gesture_id.value == gesture_id_link.value)
      {
        return;
      }
    }

    // 最後のidならclean up
    for (auto &gesture : _gesture_map)
    {
      if (gesture.gesture_id.value == gesture_id_link.value)
      {
        if (gesture.pre_action.has_value() &&
            gesture.pre_action.value().is_pressed == true)
        {
          gesture.pre_action.value().is_pressed = false;
          gesture.pre_action.value().action->release();
          gesture.instead_of_first_gesture_millis = etl::nullopt;
        }
        gesture.total_distance_x = 0;
        gesture.total_distance_y = 0;
      }
    }
  }

  //------------------------------------------------------------------+
  // RotateEncoder
  //------------------------------------------------------------------+
  void HidEngineClass::rotateEncoder_body(EncoderId encoder_id)
  {
    if (_read_encoder_step_cb == nullptr)
    {
      return;
    }

    Hid.waitReady();
    int16_t step = 0;
    _read_encoder_step_cb(encoder_id, step);

    if (step == 0)
    {
      return;
    }

    BeforeEncoderRotateEventListener::_notifyBeforeEncoderRotate(encoder_id, step);

    EncoderShift *encoder = getCurrentEncoder(encoder_id);

    if (encoder == nullptr)
    {
      return;
    }

    encoder->total_step = etl::clamp(encoder->total_step + step, INT32_MIN, INT32_MAX);
    uint8_t n_times = std::min<int32_t>(abs(encoder->total_step) / encoder->step, UINT8_MAX);

    if (n_times == 0)
    {
      return;
    }

    encoder->total_step %= encoder->step;

    if (encoder->pre_action.has_value() &&
        encoder->pre_action.value().is_pressed == false)
    {
      encoder->pre_action.value().is_pressed = true;
      encoder->pre_action.value().action->press();

      if (encoder->pre_action.value().timing == Timing::InsteadOfFirstAction)
      {
        n_times--;
        if (n_times == 0)
        {
          return;
        }
      }
    }

    if (step > 0)
    {
      ActionTapper.tap(encoder->clockwise_action, n_times);
    }
    else
    {
      ActionTapper.tap(encoder->counterclockwise_action, n_times);
    }
  }

  EncoderShift *HidEngineClass::getCurrentEncoder(EncoderId encoder_id)
  {
    for (auto &started_encoder_shift_id : _started_encoder_shift_id_list)
    {
      for (auto &encoder : _encoder_shift_map)
      {
        if ((started_encoder_shift_id.value == encoder.encoder_shift_id.value) && (encoder.encoder_id == encoder_id))
        {
          return &encoder;
        }
      }
    }

    for (auto &encoder : _encoder_map)
    {
      if (encoder.encoder_id == encoder_id)
      {
        return &encoder;
      }
    }

    return nullptr;
  }

  void HidEngineClass::startEncoderShift(EncoderShiftIdLink &encoder_shift_id_link)
  {
    if (encoder_shift_id_link.is_linked())
    {
      return;
    }

    _started_encoder_shift_id_list.push_front(encoder_shift_id_link);

    // pre_action
    for (auto &encoder : _encoder_shift_map)
    {
      if (encoder.encoder_shift_id.value == encoder_shift_id_link.value &&
          encoder.pre_action.has_value() &&
          encoder.pre_action.value().timing == Timing::Immediately &&
          encoder.pre_action.value().is_pressed == false)
      {
        encoder.pre_action.value().is_pressed = true;
        encoder.pre_action.value().action->press();
      }
    }
  }

  void HidEngineClass::stopEncoderShift(EncoderShiftIdLink &encoder_shift_id_link)
  {
    if (encoder_shift_id_link.is_linked() == false)
    {
      return;
    }

    erase_link_from_list(_started_encoder_shift_id_list, encoder_shift_id_link);

    // 同じidが同時に押されることもあり得るので最後に押されていたかチェック
    for (auto &started_encoder_id : _started_encoder_shift_id_list)
    {
      if (started_encoder_id.value == encoder_shift_id_link.value)
      {
        return;
      }
    }

    // 最後のidならclean up
    for (auto &encoder : _encoder_shift_map)
    {
      if (encoder.encoder_shift_id.value == encoder_shift_id_link.value)
      {
        if (encoder.pre_action.has_value() &&
            encoder.pre_action.value().is_pressed == true)
        {
          encoder.pre_action.value().is_pressed = false;
          encoder.pre_action.value().action->release();
        }
        encoder.total_step = 0;
      }
    }
  }

  HidEngineClass HidEngine;

}
