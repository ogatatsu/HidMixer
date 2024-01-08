
#pragma once

#include "etl/intrusive_links.h"
#include "etl/intrusive_list.h"
#include "etl/optional.h"
#include "etl/span.h"
#include "gsl/gsl-lite.hpp"
#include <ctype.h>

namespace HidMixer::HidEngineDetail
{
  using BeforeOtherKeyPressEventListenerLink = etl::bidirectional_link<0>;
  using BeforePointerMoveEventListenerLink = etl::bidirectional_link<1>;
  using BeforeEncoderRotateEventListenerLink = etl::bidirectional_link<2>;
  using BeforeGestureEventListenerLink = etl::bidirectional_link<3>;
  using PressedKeyLink = etl::bidirectional_link<4>;
  using StartedKeyShiftIdLink = etl::bidirectional_link<5>;
  using StartedGestureIdLink = etl::bidirectional_link<6>;
  using StartedEncoderShiftIdLink = etl::bidirectional_link<7>;
  using SuccessComboLink = etl::bidirectional_link<8>;

  //------------------------------------------------------------------+
  // Id
  //------------------------------------------------------------------+

  struct KeyShiftId
  {
    uint8_t value;

    bool operator==(const KeyShiftId &rhs) const { return value == rhs.value; }
    bool operator!=(const KeyShiftId &rhs) const { return value != rhs.value; }
  };

  struct PointingDeviceId
  {
    uint8_t value;

    bool operator==(const PointingDeviceId &rhs) const { return value == rhs.value; }
    bool operator!=(const PointingDeviceId &rhs) const { return value != rhs.value; }
  };

  struct GestureId
  {
    uint8_t value;

    bool operator==(const GestureId &rhs) const { return value == rhs.value; }
    bool operator!=(const GestureId &rhs) const { return value != rhs.value; }
  };

  struct EncoderId
  {
    uint8_t value;

    bool operator==(const EncoderId &rhs) const { return value == rhs.value; }
    bool operator!=(const EncoderId &rhs) const { return value != rhs.value; }
  };

  struct EncoderShiftId
  {
    uint8_t value;

    bool operator==(const EncoderShiftId &rhs) const { return value == rhs.value; }
    bool operator!=(const EncoderShiftId &rhs) const { return value != rhs.value; }
  };

  //------------------------------------------------------------------+
  // Action
  //------------------------------------------------------------------+
  class Action
  {
  public:
    Action();
    void press();
    void release();
    uint8_t tap(uint8_t n_times = 1);

    virtual void init(etl::optional<uint8_t> key_id) { _key_id = key_id; }
    etl::optional<uint8_t> getKeyId() { return _key_id; }

  protected:
    virtual void onPress(){};
    virtual void onRelease(){};
    virtual uint8_t onTap(uint8_t n_times)
    {
      onPress();
      onRelease();
      return 1;
    };

  private:
    etl::optional<uint8_t> _key_id;
    bool _is_pressed;
  };

  using ActionPtr = Action *;
  using NotNullActionPtr = gsl::not_null<Action *>;

  //------------------------------------------------------------------+
  // BeforeOtherKeyPressEventListener
  //------------------------------------------------------------------+
  class BeforeOtherKeyPressEventListener : public BeforeOtherKeyPressEventListenerLink
  {
  public:
    BeforeOtherKeyPressEventListener(Action *action);
    static void _notifyBeforeOtherKeyPress(uint8_t key_id);

  protected:
    bool startListenBeforeOtherKeyPress();
    bool stopListenBeforeOtherKeyPress();
    virtual void onBeforeOtherKeyPress(uint8_t key_id) = 0;

  private:
    using List = etl::intrusive_list<BeforeOtherKeyPressEventListener, BeforeOtherKeyPressEventListenerLink>;
    static List _listener_list;

    Action *_action;
  };

  //------------------------------------------------------------------+
  // BeforePointerMoveEventListener
  //------------------------------------------------------------------+
  class BeforePointerMoveEventListener : public BeforePointerMoveEventListenerLink
  {
  public:
    BeforePointerMoveEventListener();
    static void _notifyBeforePointerMove(PointingDeviceId pointing_device_id, int16_t delta_x, int16_t delta_y);

  protected:
    bool startListenBeforePointerMove();
    bool stopListenBeforePointerMove();
    virtual void onBeforePointerMove(PointingDeviceId pointing_device_id, int16_t delta_x, int16_t delta_y) = 0;

  private:
    using List = etl::intrusive_list<BeforePointerMoveEventListener, BeforePointerMoveEventListenerLink>;
    static List _listener_list;
  };

  //------------------------------------------------------------------+
  // BeforeEncoderRotateEventListener
  //------------------------------------------------------------------+
  class BeforeEncoderRotateEventListener : public BeforeEncoderRotateEventListenerLink
  {
  public:
    BeforeEncoderRotateEventListener();
    static void _notifyBeforeEncoderRotate(EncoderId encoder_id, int16_t step);

  protected:
    bool startListenBeforeEncoderRotate();
    bool stopListenBeforeEncoderRotate();
    virtual void onBeforeRotateEncoder(EncoderId encoder_id, int16_t step) = 0;

  private:
    using List = etl::intrusive_list<BeforeEncoderRotateEventListener, BeforeEncoderRotateEventListenerLink>;
    static List _listener_list;
  };

  //------------------------------------------------------------------+
  // BeforeGestureEventListener
  //------------------------------------------------------------------+
  class BeforeGestureEventListener : public BeforeGestureEventListenerLink
  {
  public:
    BeforeGestureEventListener();
    static void _notifyBeforeGesture(GestureId gesture_id, PointingDeviceId pointing_device_id);

  protected:
    bool startListenBeforeGesture();
    bool stopListenBeforeGesture();
    virtual void onBeforeGesture(GestureId gesture_id, PointingDeviceId pointing_device_id) = 0;

  private:
    using List = etl::intrusive_list<BeforeGestureEventListener, BeforeGestureEventListenerLink>;
    static List _listener_list;
  };

  // ------------------------------------------------------------------+
  // Key
  // ------------------------------------------------------------------+
  enum class Timing : uint8_t
  {
    Immediately,
    JustBeforeFirstAction,
    InsteadOfFirstAction,
  };

  enum class KeymapOverlay : uint8_t
  {
    Enable,
    Disable,
  };

  struct PreAction
  {
    PreAction(NotNullActionPtr action, Timing timing)
        : action(action), timing(timing), is_pressed(false) {}

    // const NotNullActionPtr action;
    // const Timing timing;
    NotNullActionPtr action;
    Timing timing;
    bool is_pressed;
  };

  struct Key : public PressedKeyLink
  {
    Key(uint8_t key_id, NotNullActionPtr action)
        : PressedKeyLink(), key_id(key_id), action(action) {}

    const uint8_t key_id;
    const NotNullActionPtr action;
  };

  struct KeyShift
  {
    KeyShift(KeyShiftId key_shift_id,
             KeymapOverlay keymap_overlay,
             etl::span<Key> keymap,
             etl::optional<PreAction> pre_action = etl::nullopt)
        : key_shift_id(key_shift_id),
          keymap_overlay(keymap_overlay),
          keymap(keymap),
          pre_action(pre_action) {}

    const KeyShiftId key_shift_id;
    const KeymapOverlay keymap_overlay;
    const etl::span<Key> keymap;
    etl::optional<PreAction> pre_action;
  };

  struct KeyShiftIdLink : public StartedKeyShiftIdLink
  {
    KeyShiftIdLink(uint8_t value = 0) : StartedKeyShiftIdLink(), value(value) {}
    const uint8_t value;
  };

  // ------------------------------------------------------------------+
  // Combo
  // ------------------------------------------------------------------+
  enum class ComboBehavior : uint8_t
  {
    AnyOrder_SlowRelease = 0b00,
    AnyOrder_FastRelease = 0b01,
    SpecifiedOrder_SlowRelease = 0b10,
    SpecifiedOrder_FastRelease = 0b11,
  };

  struct Combo : public SuccessComboLink
  {
    Combo(uint8_t first_key_id,
          uint8_t second_key_id,
          NotNullActionPtr action,
          uint32_t combo_term_ms,
          ComboBehavior combo_behavior)
        : SuccessComboLink(),
          first_id(first_key_id),
          second_id(second_key_id),
          action(action),
          combo_term_ms(combo_term_ms),
          behavior(combo_behavior)
    {
    }

    const uint8_t first_id;
    const uint8_t second_id;
    const NotNullActionPtr action;
    const uint32_t combo_term_ms;
    const ComboBehavior behavior;

    bool first_id_released;
    bool second_id_released;

    bool isSpecifiedOrder() { return static_cast<uint8_t>(behavior) & 0b10; }
    bool isAnyOrder() { return !isSpecifiedOrder(); }
    bool isFastRelease() { return static_cast<uint8_t>(behavior) & 0b01; }
    bool isSlowRelease() { return !isFastRelease(); }

    bool isMatchFirstId(uint8_t id)
    {
      if (isSpecifiedOrder())
      {
        return first_id == id;
      }
      return first_id == id || second_id == id;
    }

    bool isMatchIds(uint8_t fst, uint8_t snd)
    {
      if (isSpecifiedOrder())
      {
        return first_id == fst && second_id == snd;
      }
      return (first_id == fst && second_id == snd) || (first_id == snd && second_id == fst);
    }
  };

  // ------------------------------------------------------------------+
  // Gesture
  // ------------------------------------------------------------------+
  enum class AngleSnap : uint8_t
  {
    Enable,
    Disable,
  };

  struct Gesture
  {
    Gesture(GestureId gesture_id,
            PointingDeviceId pointing_device_id,
            uint16_t distance,
            AngleSnap angle_snap,
            NotNullActionPtr up_action,
            NotNullActionPtr down_action,
            NotNullActionPtr left_action,
            NotNullActionPtr right_action,
            etl::optional<PreAction> pre_action = etl::nullopt)
        : gesture_id(gesture_id),
          pointing_device_id(pointing_device_id),
          distance(distance),
          angle_snap(angle_snap),
          up_action(up_action),
          down_action(down_action),
          left_action(left_action),
          right_action(right_action),
          pre_action(pre_action),
          total_distance_x(0),
          total_distance_y(0),
          instead_of_first_gesture_millis(etl::nullopt)
    {
    }

    const GestureId gesture_id;
    const PointingDeviceId pointing_device_id;
    const uint16_t distance;
    const AngleSnap angle_snap;
    const NotNullActionPtr up_action;
    const NotNullActionPtr down_action;
    const NotNullActionPtr left_action;
    const NotNullActionPtr right_action;

    etl::optional<PreAction> pre_action;
    int32_t total_distance_x;
    int32_t total_distance_y;
    etl::optional<uint32_t> instead_of_first_gesture_millis;
  };

  struct GestureIdLink : public StartedGestureIdLink
  {
    GestureIdLink(uint8_t value = 0) : StartedGestureIdLink(), value(value) {}
    const uint8_t value;
  };

  // ------------------------------------------------------------------+
  // Encoder
  // ------------------------------------------------------------------+
  struct EncoderShift
  {
    EncoderShift(EncoderShiftId encoder_shift_id,
                 EncoderId encoder_id,
                 uint8_t step,
                 NotNullActionPtr counterclockwise_action,
                 NotNullActionPtr clockwise_action,
                 etl::optional<PreAction> pre_action = etl::nullopt)
        : encoder_shift_id(encoder_shift_id),
          encoder_id(encoder_id),
          step(step),
          counterclockwise_action(counterclockwise_action),
          clockwise_action(clockwise_action),
          pre_action(pre_action)
    {
    }

    const EncoderShiftId encoder_shift_id;
    const EncoderId encoder_id;
    const uint8_t step;
    const NotNullActionPtr counterclockwise_action;
    const NotNullActionPtr clockwise_action;

    int32_t total_step;
    etl::optional<PreAction> pre_action;
  };

  struct Encoder : public EncoderShift
  {
    Encoder(EncoderId encoder_id,
            uint8_t step,
            NotNullActionPtr counterclockwise_action,
            NotNullActionPtr clockwise_action)
        : EncoderShift(EncoderShiftId{0},
                       encoder_id,
                       step,
                       counterclockwise_action,
                       clockwise_action,
                       etl::nullopt)
    {
    }
  };

  struct EncoderShiftIdLink : public StartedEncoderShiftIdLink
  {
    EncoderShiftIdLink(uint8_t value = 0) : StartedEncoderShiftIdLink(), value(value) {}
    const uint8_t value;
  };

}
