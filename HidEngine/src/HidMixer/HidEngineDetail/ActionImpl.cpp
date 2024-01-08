
#include "HidMixer/HidEngineDetail/ActionImpl.h"
#include "HidMixer/HidEngineDetail/ActionTapper.h"
#include "HidMixer/HidEngineDetail/HidCore.h"
#include "HidMixer/HidEngineDetail/utility.h"

namespace HidMixer::HidEngineDetail
{

  //------------------------------------------------------------------+
  // CharacterKey
  //------------------------------------------------------------------+
  CharacterKey::CharacterKey(CharacterKeyCode code) : _code(code)
  {
  }

  void CharacterKey::onPress()
  {
    Hid.keyPress(_code);
  }

  void CharacterKey::onRelease()
  {
    Hid.keyRelease(_code);
  }

  //------------------------------------------------------------------+
  // Modifiers
  //------------------------------------------------------------------+
  Modifiers::Modifiers(ModifiersCode code) : _code(code)
  {
  }

  void Modifiers::onPress()
  {
    Hid.modifiersPress(_code);
  }

  void Modifiers::onRelease()
  {
    Hid.modifiersRelease(_code);
  }

  //------------------------------------------------------------------+
  // CombinationKey
  //------------------------------------------------------------------+
  CombinationKey::CombinationKey(CombinationKeyCode code) : _code(code)
  {
  }

  void CombinationKey::onPress()
  {
    Hid.modifiersPress(_code.modifiers);
    Hid.keyPress(_code.character_key);
  }

  void CombinationKey::onRelease()
  {
    Hid.keyRelease(_code.character_key);
    Hid.modifiersRelease(_code.modifiers);
  }

  //------------------------------------------------------------------+
  // ConsumerControl
  //------------------------------------------------------------------+
  ConsumerControl::ConsumerControl(ConsumerControlCode code) : _code(code)
  {
  }

  void ConsumerControl::onPress()
  {
    Hid.consumerControlPress(_code);
  }

  void ConsumerControl::onRelease()
  {
    Hid.consumerControlRelease();
  }

  //------------------------------------------------------------------+
  // SystemControl
  //------------------------------------------------------------------+
  SystemControl::SystemControl(SystemControlCode code) : _code(code)
  {
  }

  void SystemControl::onPress()
  {
    Hid.systemControlPress(_code);
  }

  void SystemControl::onRelease()
  {
    Hid.systemControlRelease();
  }

  //------------------------------------------------------------------+
  // Tap
  //------------------------------------------------------------------+
  Tap::Tap(NotNullActionPtr action, uint8_t n_times, uint16_t tap_speed_ms)
      : _action(action), _n_times(n_times), _tap_speed_ms(tap_speed_ms)
  {
  }

  void Tap::init(etl::optional<uint8_t> key_id)
  {
    Action::init(key_id);
    _action->init(key_id);
  }

  void Tap::onPress()
  {
    ActionTapper.tap(_action, _n_times, _tap_speed_ms);
  }

  //------------------------------------------------------------------+
  // TapWhenReleased
  //------------------------------------------------------------------+
  TapWhenReleased::TapWhenReleased(NotNullActionPtr action, uint8_t n_times, uint16_t tap_speed_ms)
      : _action(action), _n_times(n_times), _tap_speed_ms(tap_speed_ms)
  {
  }

  void TapWhenReleased::init(etl::optional<uint8_t> key_id)
  {
    Action::init(key_id);
    _action->init(key_id);
  }

  void TapWhenReleased::onRelease()
  {
    ActionTapper.tap(_action, _n_times, _tap_speed_ms);
  }

  //------------------------------------------------------------------+
  // TapDance
  //------------------------------------------------------------------+
  TapDance::TapDance(etl::span<Pair> pairs,
                     etl::span<PointingDeviceId> pointing_device_ids,
                     uint16_t move_threshold,
                     uint32_t tapping_term_ms)
      : TimerMixin(),
        BeforeOtherKeyPressEventListener(this),
        BeforePointerMoveEventListener(),
        _pairs(pairs),
        _pointing_device_ids(pointing_device_ids),
        _move_threshold(move_threshold),
        _tapping_term_ms(tapping_term_ms),
        _delta_x_sum(0),
        _delta_y_sum(0),
        _idx_count(-1),
        _state(State::Unexecuted)
  {
  }

  void TapDance::init(etl::optional<uint8_t> key_id)
  {
    Action::init(key_id);

    for (auto &pair : _pairs)
    {
      pair.hold_action->init(key_id);
      if (pair.tap_action != nullptr)
      {
        pair.tap_action->init(key_id);
      }
    }
  }

  void TapDance::performTap()
  {
    Action &cmd = (_pairs[_idx_count].tap_action != nullptr)
                      ? *(_pairs[_idx_count].tap_action)
                      : *(_pairs[_idx_count].hold_action.get());
    cmd.press();
    cmd.release();

    _idx_count = -1;
    stopTimer();
    stopListenBeforeOtherKeyPress();
    stopListenBeforeEncoderRotate();
    stopListenBeforePointerMove();
  }

  void TapDance::performHoldPress()
  {
    _performed_action = _pairs[_idx_count].hold_action;
    _performed_action->press();

    _idx_count = -1;
    stopTimer();
    stopListenBeforeOtherKeyPress();
    stopListenBeforeEncoderRotate();
    stopListenBeforePointerMove();
  }

  void TapDance::performHoldRelease()
  {
    _performed_action->release();
  }

  bool TapDance::checkMoveThreshold(BeforePointerMoveArgs args)
  {
    for (auto &id : _pointing_device_ids)
    {
      if (id == args.pointing_device_id)
      {
        _delta_x_sum = etl::clamp(_delta_x_sum + args.delta_x, INT16_MIN, INT16_MAX);
        _delta_y_sum = etl::clamp(_delta_y_sum + args.delta_y, INT16_MIN, INT16_MAX);
        if (abs(_delta_x_sum) >= _move_threshold || abs(_delta_y_sum) >= _move_threshold)
        {
          return true;
        }
      }
    }
    return false;
  }

  void TapDance::processTapDance(Event event, ArgsType args)
  {
    switch (Context(event, _state))
    {
    case Context(Event::Press, State::Unexecuted):
    {
      _state = State::Pressed;
      _idx_count++;
      startListenBeforeOtherKeyPress();
      if (_pointing_device_ids.empty() == false)
      {
        _delta_x_sum = 0;
        _delta_y_sum = 0;
        startListenBeforePointerMove();
      }
      startListenBeforeEncoderRotate();
      startTimer(_tapping_term_ms);
    }
    break;

    case Context(Event::Press, State::TapOrNextAction):
    {
      _idx_count++;
      if (_idx_count == _pairs.size() - 1 &&
          _pairs[_idx_count].tap_action == nullptr)
      {
        _state = State::DecidedToHold;
        performHoldPress();
      }
      else
      {
        _state = State::Pressed;
        startTimer(_tapping_term_ms);
      }
    }
    break;

    case Context(Event::Release, State::Pressed):
    {
      if (_idx_count == _pairs.size() - 1)
      {
        _state = State::Unexecuted;
        performTap();
      }
      else
      {
        _state = State::TapOrNextAction;
        startTimer(_tapping_term_ms);
      }
    }
    break;

    case Context(Event::Release, State::DecidedToHold):
    {
      _state = State::Unexecuted;
      performHoldRelease();
    }
    break;

    case Context(Event::Timer, State::Pressed):
    {
      _state = State::DecidedToHold;
      performHoldPress();
    }
    break;

    case Context(Event::Timer, State::TapOrNextAction):
    {
      _state = State::Unexecuted;
      performTap();
    }
    break;

    case Context(Event::BeforeOtherKeyPress, State::Pressed):
    {
      _state = State::DecidedToHold;
      performHoldPress();
    }
    break;

    case Context(Event::BeforeOtherKeyPress, State::TapOrNextAction):
    {
      _state = State::Unexecuted;
      performTap();
    }
    break;

    case Context(Event::BeforePointerMove, State::Pressed):
    {
      if (checkMoveThreshold(etl::get<BeforePointerMoveArgs>(args)))
      {
        _state = State::DecidedToHold;
        performHoldPress();
      }
    }
    break;

    case Context(Event::BeforePointerMove, State::TapOrNextAction):
    {
      if (checkMoveThreshold(etl::get<BeforePointerMoveArgs>(args)))
      {
        _state = State::Unexecuted;
        performTap();
      }
    }
    break;

    case Context(Event::BeforeEncoderRotate, State::Pressed):
    {
      _state = State::DecidedToHold;
      performHoldPress();
    }
    break;

    case Context(Event::BeforeEncoderRotate, State::TapOrNextAction):
    {
      _state = State::Unexecuted;
      performTap();
    }
    break;

    default:
      break;
    }
  }

  void TapDance::onPress()
  {
    processTapDance(Event::Press, nullptr);
  }

  void TapDance::onRelease()
  {
    processTapDance(Event::Release, nullptr);
  }

  void TapDance::onTimer()
  {
    processTapDance(Event::Timer, nullptr);
  }

  void TapDance::onBeforeOtherKeyPress(uint8_t key_id)
  {
    BeforeOtherKeyPressArgs args{.key_id = key_id};
    processTapDance(Event::BeforeOtherKeyPress, args);
  }

  void TapDance::onBeforePointerMove(PointingDeviceId pointing_device_id, int16_t delta_x, int16_t delta_y)
  {
    BeforePointerMoveArgs args{.pointing_device_id = pointing_device_id, .delta_x = delta_x, .delta_y = delta_y};
    processTapDance(Event::BeforePointerMove, args);
  }

  void TapDance::onBeforeRotateEncoder(EncoderId encoder_id, int16_t step)
  {
    BeforeEncoderRotateArgs args{.encoder_id = encoder_id, .step = step};
    processTapDance(Event::BeforeEncoderRotate, args);
  }

  //------------------------------------------------------------------+
  // PointerMove
  //------------------------------------------------------------------+
  PointerMove::PointerMove(int16_t x, int16_t y)
      : _x(x), _y(y), _max_n_times(std::min(32767 / std::max(abs(_x), abs(_y)), UINT8_MAX))
  {
  }

  void PointerMove::onPress()
  {
    Hid.pointerMove(_x, _y);
  }

  uint8_t PointerMove::onTap(uint8_t n_times)
  {
    uint8_t actual_n_times = std::min(n_times, _max_n_times);
    Hid.pointerMove(_x * actual_n_times, _y * actual_n_times);
    return actual_n_times;
  }

  //------------------------------------------------------------------+
  // PointingScroll
  //------------------------------------------------------------------+
  PointingScroll::PointingScroll(int8_t scroll, int8_t pan)
      : _scroll(scroll), _pan(pan), _max_n_times(127 / std::max(abs(_scroll), abs(_pan)))
  {
  }

  void PointingScroll::onPress()
  {
    Hid.pointingScroll(_scroll, _pan);
  }

  uint8_t PointingScroll::onTap(uint8_t n_times)
  {
    uint8_t actual_n_times = std::min(n_times, _max_n_times);
    Hid.pointingScroll(_scroll * actual_n_times, _pan * actual_n_times);
    return actual_n_times;
  }

  //------------------------------------------------------------------+
  // PointingButton
  //------------------------------------------------------------------+
  PointingButton::PointingButton(PointingButtonsCode code) : _code(code)
  {
  }

  void PointingButton::onPress()
  {
    Hid.pointingButtonsPress(_code);
  }

  void PointingButton::onRelease()
  {
    Hid.pointingButtonsRelease(_code);
  }

  //------------------------------------------------------------------+
  // ConstSpeed
  //------------------------------------------------------------------+
  ConstSpeed::ConstSpeed(NotNullActionPtr action, uint32_t interval_ms, uint32_t delay_ms)
      : _action(action),
        _interval_ms(interval_ms),
        _delay_ms(delay_ms),
        _last_process_millis(0),
        _last_action_millis(0),
        _next_delay_ms(0)
  {
  }

  void ConstSpeed::init(etl::optional<uint8_t> key_id)
  {
    Action::init(key_id);
    _action->init(key_id);
  }

  void ConstSpeed::onPress()
  {
    uint32_t current_millis = millis();

    if (static_cast<uint32_t>(current_millis - _last_process_millis) >= _delay_ms)
    {
      _action->press();
      _last_action_millis = current_millis;
      _next_delay_ms = _interval_ms;
    }
    else if (static_cast<uint32_t>(current_millis - _last_action_millis) >= _next_delay_ms)
    {
      _action->press();
      _last_action_millis = current_millis;
      _next_delay_ms = _delay_ms;
    }
    _last_process_millis = current_millis;
  }

  void ConstSpeed::onRelease()
  {
    _action->release();
  }

  uint8_t ConstSpeed::onTap(uint8_t n_times)
  {
    press();
    release();
    return UINT8_MAX;
  }

  //------------------------------------------------------------------+
  // If
  //------------------------------------------------------------------+
  If::If(bool (*func)(), NotNullActionPtr true_action, NotNullActionPtr false_action)
      : _func(func), _true_action(true_action), _false_action(false_action)
  {
  }

  void If::init(etl::optional<uint8_t> key_id)
  {
    Action::init(key_id);
    _true_action->init(key_id);
    _false_action->init(key_id);
  }

  void If::onPress()
  {
    _performed_action = _func() ? _true_action : _false_action;
    _performed_action->press();
  }

  void If::onRelease()
  {
    _performed_action->release();
  }

  uint8_t If::onTap(uint8_t n_times)
  {
    auto cmd = _func() ? _true_action : _false_action;
    return cmd->tap(n_times);
  }

  //------------------------------------------------------------------+
  // Multi
  //------------------------------------------------------------------+
  Multi::Multi(etl::span<NotNullActionPtr> actions)
      : _actions(actions)
  {
  }

  void Multi::init(etl::optional<uint8_t> key_id)
  {
    Action::init(key_id);

    for (auto &action : _actions)
    {
      action->init(key_id);
    }
  }

  void Multi::onPress()
  {
    for (auto &action : _actions)
    {
      action->press();
    }
  }

  void Multi::onRelease()
  {
    for (auto &action : _actions)
    {
      action->release();
    }
  }

  //------------------------------------------------------------------+
  // Toggle
  //------------------------------------------------------------------+
  Toggle::Toggle(NotNullActionPtr action)
      : _action(action), _is_pressed(false)
  {
  }

  void Toggle::init(etl::optional<uint8_t> key_id)
  {
    Action::init(key_id);
    _action->init(key_id);
  }

  void Toggle::onPress()
  {
    if (_is_pressed == false)
    {
      _action->press();
    }
    else
    {
      _action->release();
    }

    _is_pressed = !_is_pressed;
  }

  //------------------------------------------------------------------+
  // Repeat
  //------------------------------------------------------------------+
  Repeat::Repeat(NotNullActionPtr action, uint32_t delay_ms, uint32_t interval_ms)
      : TimerMixin(), _action(action), _delay_ms(delay_ms), _interval_ms(interval_ms)
  {
  }

  void Repeat::init(etl::optional<uint8_t> key_id)
  {
    Action::init(key_id);
    _action->init(key_id);
  }

  void Repeat::onPress()
  {
    ActionTapper.tap(_action);
    startTimer(_delay_ms);
  }

  void Repeat::onRelease()
  {
    stopTimer();
  }

  void Repeat::onTimer()
  {
    ActionTapper.tap(_action);
    startTimer(_interval_ms);
  }

  //------------------------------------------------------------------+
  // NoOperation
  //------------------------------------------------------------------+
  uint8_t NoOperation::onTap(uint8_t n_times)
  {
    return UINT8_MAX;
  }

  //------------------------------------------------------------------+
  // MapShift
  //------------------------------------------------------------------+
  MapShift::MapShift(etl::span<MapShiftIdLink> map_shift_id_links) : _map_shift_id_links(map_shift_id_links)
  {
  }

  void MapShift::onPress()
  {
    for (auto &shift_id_link : _map_shift_id_links)
    {
      if (auto key_shift_id_link = etl::get_if<KeyShiftIdLink>(&shift_id_link))
      {
        HidEngine.startKeyShift(*key_shift_id_link);
      }
      else if (auto encoder_shift_id_link = etl::get_if<EncoderShiftIdLink>(&shift_id_link))
      {
        HidEngine.startEncoderShift(*encoder_shift_id_link);
      }
      else if (auto gesture_id_link = etl::get_if<GestureIdLink>(&shift_id_link))
      {
        HidEngine.startGesture(*gesture_id_link);
      }
    }
  }

  void MapShift::onRelease()
  {
    for (auto &shift_id_link : _map_shift_id_links)
    {
      if (auto key_shift_id_link = etl::get_if<KeyShiftIdLink>(&shift_id_link))
      {
        HidEngine.stopKeyShift(*key_shift_id_link);
      }
      else if (auto encoder_shift_id_link = etl::get_if<EncoderShiftIdLink>(&shift_id_link))
      {
        HidEngine.stopEncoderShift(*encoder_shift_id_link);
      }
      else if (auto gesture_id_link = etl::get_if<GestureIdLink>(&shift_id_link))
      {
        HidEngine.stopGesture(*gesture_id_link);
      }
    }
  }

  MapShiftIdLink MapShift::IdToLink(MapShiftId map_shift_id)
  {
    if (auto key_shift_id = etl::get_if<KeyShiftId>(&map_shift_id))
    {
      return KeyShiftIdLink{key_shift_id->value};
    }
    else if (auto encoder_shift_id = etl::get_if<EncoderShiftId>(&map_shift_id))
    {
      return EncoderShiftIdLink{encoder_shift_id->value};
    }
    else
    {
      auto gesture_id = etl::get<GestureId>(map_shift_id);
      return GestureIdLink{gesture_id.value};
    }
  }

}
