
#pragma once

#include "HidMixer/HidEngineDetail/TimerMixin.h"
#include "HidMixer/HidEngineDetail/types.h"
#include "HidMixer/HidReporter.h"
#include "HidMixer/Set.h"
#include <tuple>

namespace HidMixer::HidEngineDetail
{

  class HidEngineClass
  {
    friend class HidEngineTaskClass;

  public:
    using read_pointer_delta_callback_t = void (*)(PointingDeviceId pointing_device_id, int16_t &delta_x, int16_t &delta_y);
    using read_encoder_step_callback_t = void (*)(EncoderId encoder_id, int16_t &step);

#ifdef configUSE_CORE_AFFINITY
    static void start(UBaseType_t task_priority, UBaseType_t core_affinity_mask);
#else
    static void start(UBaseType_t task_priority);
#endif

    static void setKeymap(etl::span<Key> keymap);
    static void setKeyShiftMap(etl::span<KeyShift> key_shift_map);
    static void setComboMap(etl::span<Combo> combo_map);
    static void setGestureMap(etl::span<Gesture> gesture_map);
    static void setEncoderMap(etl::span<Encoder> encoder_map);
    static void setEncoderShiftMap(etl::span<EncoderShift> encoder_shift_map);
    static void setHidReporter(HidReporter *hid_reporter);
    static void applyToKeymap(const Set &key_ids);
    static void movePointer(PointingDeviceId pointing_device_id);
    static void rotateEncoder(EncoderId encoder_id);
    static void setReadPointerDeltaCallback(read_pointer_delta_callback_t cb);
    static void setReadEncoderStepCallback(read_encoder_step_callback_t cb);

    static void startKeyShift(KeyShiftIdLink &key_shift_id_link);
    static void stopKeyShift(KeyShiftIdLink &key_shift_id_link);
    static void startGesture(GestureIdLink &gesture_id_link);
    static void stopGesture(GestureIdLink &gesture_id_link);
    static void startEncoderShift(EncoderShiftIdLink &encoder_shift_id_link);
    static void stopEncoderShift(EncoderShiftIdLink &encoder_shift_id_link);

  private:
    enum class Event
    {
      Press,
      Release,
      ComboInterruption,
    };
    static void initAction();

    static void applyToKeymap_body(Set &key_ids);
    static void processComboAndKey(Event event, etl::optional<uint8_t> key_id);
    static void performKeyPress(uint8_t key_id);
    static void performKeyRelease(uint8_t key_id);
    static std::tuple<KeyShift *, Key *> getCurrentKey(uint8_t key_id);

    static void movePointer_body(PointingDeviceId pointing_device_id);
    static Gesture *getCurrentGesture(PointingDeviceId pointing_device_id);
    static void processGesture(Gesture &gesture, int16_t delta_x, int16_t delta_y);
    static void processGestureX(Gesture &gesture);
    static void processGestureY(Gesture &gesture);
    static void processGesturePreAction(Gesture &gesture, uint8_t &n_times);

    static void rotateEncoder_body(EncoderId encoder_id);
    static EncoderShift *getCurrentEncoder(EncoderId encoder_id);

    static etl::span<Key> _keymap;
    static etl::span<KeyShift> _key_shift_map;
    static etl::span<Combo> _combo_map;
    static etl::span<Gesture> _gesture_map;
    static etl::span<Encoder> _encoder_map;
    static etl::span<EncoderShift> _encoder_shift_map;

    static read_pointer_delta_callback_t _read_pointer_delta_cb;
    static read_encoder_step_callback_t _read_encoder_step_cb;

    class ComboInterruptionEvent : public TimerMixin,
                                   public BeforePointerMoveEventListener,
                                   public BeforeEncoderRotateEventListener
    {
    public:
      void start(unsigned int combo_term_ms)
      {
        _move_pointer_count = 0;
        startTimer(combo_term_ms);
        startListenBeforePointerMove();
        startListenBeforeEncoderRotate();
      }

      void stop()
      {
        stopTimer();
        stopListenBeforePointerMove();
        stopListenBeforeEncoderRotate();
      }

    protected:
      void onTimer() override { processComboAndKey(Event::ComboInterruption, etl::nullopt); }
      void onBeforeRotateEncoder(EncoderId, int16_t) override { processComboAndKey(Event::ComboInterruption, etl::nullopt); }
      void onBeforePointerMove(PointingDeviceId, int16_t, int16_t) override
      {
        if (++_move_pointer_count >= HID_ENGINE_COMBO_INTERRUPTION_MOVE_POINTER_COUNT)
        {
          processComboAndKey(Event::ComboInterruption, etl::nullopt);
        }
      }

    private:
      uint8_t _move_pointer_count;
    };
    static ComboInterruptionEvent _combo_interruption_event;

    static etl::intrusive_list<Key, PressedKeyLink> _pressed_key_list;
    static etl::intrusive_list<KeyShiftIdLink, StartedKeyShiftIdLink> _started_key_shift_id_list;
    static etl::intrusive_list<GestureIdLink, StartedGestureIdLink> _started_gesture_id_list;
    static etl::intrusive_list<EncoderShiftIdLink, StartedEncoderShiftIdLink> _started_encoder_shift_id_list;
  };

  extern HidEngineClass HidEngine;

}
