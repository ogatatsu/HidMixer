
#pragma once

#include "HidMixer/HidEngineDetail/KeyCode.h"
#include "HidMixer/HidEngineDetail/TimerMixin.h"
#include "HidMixer/HidEngineDetail/types.h"
#include "etl/variant.h"

namespace HidMixer::HidEngineDetail
{

  //------------------------------------------------------------------+
  // CharacterKey
  //------------------------------------------------------------------+
  class CharacterKey : public Action
  {
  public:
    CharacterKey(CharacterKeyCode code);

  protected:
    void onPress() override;
    void onRelease() override;

  private:
    const CharacterKeyCode _code;
  };

  //------------------------------------------------------------------+
  // Modifiers
  //------------------------------------------------------------------+
  class Modifiers : public Action
  {
  public:
    Modifiers(ModifiersCode code);

  protected:
    void onPress() override;
    void onRelease() override;

  private:
    const ModifiersCode _code;
  };

  //------------------------------------------------------------------+
  // CombinationKey
  //------------------------------------------------------------------+
  class CombinationKey : public Action
  {
  public:
    CombinationKey(CombinationKeyCode code);

  protected:
    void onPress() override;
    void onRelease() override;

  private:
    const CombinationKeyCode _code;
  };

  //------------------------------------------------------------------+
  // ConsumerControl
  //------------------------------------------------------------------+
  class ConsumerControl : public Action
  {
  public:
    ConsumerControl(ConsumerControlCode code);

  protected:
    void onPress() override;
    void onRelease() override;

  private:
    const ConsumerControlCode _code;
  };

  //------------------------------------------------------------------+
  // SystemControl
  //------------------------------------------------------------------+
  class SystemControl : public Action
  {
  public:
    SystemControl(SystemControlCode code);

  protected:
    void onPress() override;
    void onRelease() override;

  private:
    const SystemControlCode _code;
  };

  // ------------------------------------------------------------------+
  // Tap
  // ------------------------------------------------------------------+
  class Tap : public Action
  {
  public:
    Tap(NotNullActionPtr action, uint8_t n_times, uint16_t tap_speed_ms);
    void init(etl::optional<uint8_t> key_id) override;

  protected:
    void onPress() override;

  private:
    const NotNullActionPtr _action;
    const uint8_t _n_times;
    const uint16_t _tap_speed_ms;
  };

  // ------------------------------------------------------------------+
  // TapWhenReleased
  // ------------------------------------------------------------------+
  class TapWhenReleased : public Action
  {
  public:
    TapWhenReleased(NotNullActionPtr action, uint8_t n_times, uint16_t tap_speed_ms);
    void init(etl::optional<uint8_t> key_id) override;

  protected:
    void onRelease() override;

  private:
    const NotNullActionPtr _action;
    const uint8_t _n_times;
    const uint16_t _tap_speed_ms;
  };

  //------------------------------------------------------------------+
  // TapDance
  //------------------------------------------------------------------+
  class TapDance : public Action,
                   public TimerMixin,
                   public BeforeOtherKeyPressEventListener,
                   public BeforePointerMoveEventListener,
                   public BeforeEncoderRotateEventListener
  {
  public:
    struct Pair
    {
      Pair(NotNullActionPtr hold_action, ActionPtr tap_action = nullptr) : hold_action(hold_action), tap_action(tap_action) {}
      const NotNullActionPtr hold_action;
      const ActionPtr tap_action;
    };

    TapDance(etl::span<Pair> pairs, etl::span<PointingDeviceId> pointing_device_ids, uint16_t move_threshold, uint32_t tapping_term_ms);
    void init(etl::optional<uint8_t> key_id) override;

  protected:
    void onPress() override;
    void onRelease() override;
    void onTimer() override;
    void onBeforeOtherKeyPress(uint8_t key_id) override;
    void onBeforePointerMove(PointingDeviceId pointing_device_id, int16_t delta_x, int16_t delta_y) override;
    void onBeforeRotateEncoder(EncoderId encoder_id, int16_t step) override;

  private:
    struct BeforeOtherKeyPressArgs
    {
      const uint8_t key_id;
    };

    struct BeforePointerMoveArgs
    {
      const PointingDeviceId pointing_device_id;
      const int16_t delta_x;
      const int16_t delta_y;
    };

    struct BeforeEncoderRotateArgs
    {
      const EncoderId encoder_id;
      const int16_t step;
    };

    using ArgsType = etl::variant<BeforeOtherKeyPressArgs, BeforePointerMoveArgs, BeforeEncoderRotateArgs, nullptr_t>;

    // clang-format off
      enum class Event : uint32_t
      {
        Press                   = 0b1,
        Release                 = 0b10,
        Timer                   = 0b100,
        BeforeOtherKeyPress     = 0b1000,
        BeforePointerMove       = 0b10000,
        BeforeEncoderRotate     = 0b100000,
      };

      enum class State : uint32_t
      {
        Unexecuted              = 0b1000000,
        Pressed                 = 0b10000000,
        TapOrNextAction         = 0b100000000,
        DecidedToHold           = 0b1000000000,
      };
    // clang-format on

    static constexpr uint32_t Context(Event action, State state)
    {
      return static_cast<uint32_t>(static_cast<uint32_t>(action) | static_cast<uint32_t>(state));
    }

    void performTap();
    void performHoldPress();
    void performHoldRelease();
    bool checkMoveThreshold(BeforePointerMoveArgs args);
    void processTapDance(Event action, ArgsType args);

    Action *_performed_action;
    const etl::span<Pair> _pairs;
    const etl::span<PointingDeviceId> _pointing_device_ids;
    const uint16_t _move_threshold;
    const uint32_t _tapping_term_ms;
    int16_t _delta_x_sum;
    int16_t _delta_y_sum;
    size_t _idx_count;
    State _state;
  };

  //------------------------------------------------------------------+
  // PointerMove
  //------------------------------------------------------------------+
  class PointerMove : public Action
  {
  public:
    PointerMove(int16_t x, int16_t y);

  protected:
    void onPress() override;
    uint8_t onTap(uint8_t n_times) override;

  private:
    const int16_t _x;
    const int16_t _y;
    const uint8_t _max_n_times;
  };

  //------------------------------------------------------------------+
  // PointingScroll
  //------------------------------------------------------------------+
  class PointingScroll : public Action
  {
  public:
    PointingScroll(int8_t scroll, int8_t pan);

  protected:
    void onPress() override;
    uint8_t onTap(uint8_t n_times) override;

  private:
    const int8_t _scroll;
    const int8_t _pan;
    const uint8_t _max_n_times;
  };

  //------------------------------------------------------------------+
  // PointingButton
  //------------------------------------------------------------------+
  class PointingButton : public Action
  {
  public:
    PointingButton(PointingButtonsCode code);

  protected:
    void onPress() override;
    void onRelease() override;

  private:
    const PointingButtonsCode _code;
  };

  //------------------------------------------------------------------+
  // ConstSpeed
  //------------------------------------------------------------------+
  class ConstSpeed : public Action
  {
  public:
    ConstSpeed(NotNullActionPtr action, uint32_t interval_ms, uint32_t delay_ms);
    void init(etl::optional<uint8_t> key_id) override;

  protected:
    void onPress() override;
    void onRelease() override;
    uint8_t onTap(uint8_t n_times) override;

  private:
    const NotNullActionPtr _action;
    const uint32_t _interval_ms;
    const uint32_t _delay_ms;
    uint32_t _last_process_millis;
    uint32_t _last_action_millis;
    uint32_t _next_delay_ms;
  };

  //------------------------------------------------------------------+
  // If
  //------------------------------------------------------------------+
  class If : public Action
  {
  public:
    If(bool (*func)(), NotNullActionPtr true_action, NotNullActionPtr false_action);
    void init(etl::optional<uint8_t> key_id) override;

  protected:
    void onPress() override;
    void onRelease() override;
    uint8_t onTap(uint8_t n_times) override;

  private:
    bool (*const _func)();
    const NotNullActionPtr _true_action;
    const NotNullActionPtr _false_action;
    Action *_performed_action;
  };

  //------------------------------------------------------------------+
  // Multi
  //------------------------------------------------------------------+
  class Multi : public Action
  {
  public:
    Multi(etl::span<NotNullActionPtr> actions);
    void init(etl::optional<uint8_t> key_id) override;

  protected:
    void onPress() override;
    void onRelease() override;

  private:
    const etl::span<NotNullActionPtr> _actions;
  };

  //------------------------------------------------------------------+
  // Toggle
  //------------------------------------------------------------------+
  class Toggle : public Action
  {
  public:
    Toggle(NotNullActionPtr action);
    void init(etl::optional<uint8_t> key_id) override;

  protected:
    void onPress() override;

  private:
    const NotNullActionPtr _action;
    bool _is_pressed;
  };

  //------------------------------------------------------------------+
  // Repeat
  //------------------------------------------------------------------+
  class Repeat : public Action, public TimerMixin
  {
  public:
    Repeat(NotNullActionPtr action, uint32_t delay_ms, uint32_t interval_ms);
    void init(etl::optional<uint8_t> key_id) override;

  protected:
    void onPress() override;
    void onRelease() override;
    void onTimer() override;

  private:
    const NotNullActionPtr _action;
    const uint32_t _delay_ms;
    const uint32_t _interval_ms;
  };

  //------------------------------------------------------------------+
  // NoOperation
  //------------------------------------------------------------------+
  class NoOperation : public Action
  {
  protected:
    uint8_t onTap(uint8_t n_times) override;
  };

  //------------------------------------------------------------------+
  // MapShift
  //------------------------------------------------------------------+
  using MapShiftId = etl::variant<KeyShiftId, EncoderShiftId, GestureId>;
  using MapShiftIdLink = etl::variant<KeyShiftIdLink, EncoderShiftIdLink, GestureIdLink>;

  class MapShift : public Action
  {
  public:
    MapShift(etl::span<MapShiftIdLink> map_shift_id_links);
    static MapShiftIdLink IdToLink(MapShiftId map_shift_id);

  protected:
    void onPress() override;
    void onRelease() override;

  private:
    const etl::span<MapShiftIdLink> _map_shift_id_links;
  };

}
