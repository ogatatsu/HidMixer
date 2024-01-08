
#pragma once

#include "HidMixer/HidEngineDetail/ActionImpl.h"
#include "consthash/cityhash64.hxx"
#include "etl/array.h"
#include "etl/vector.h"
#include <new>

namespace HidMixer::HidEngineDetail
{

  //------------------------------------------------------------------+
  // KC
  //------------------------------------------------------------------+
  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_KeyCode(CharacterKeyCode code)
  {
    static uint8_t buf[sizeof(CharacterKey)];
    return new (buf) CharacterKey(code);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_KeyCode(ModifiersCode code)
  {
    static uint8_t buf[sizeof(Modifiers)];
    return new (buf) Modifiers(code);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_KeyCode(CombinationKeyCode code)
  {
    static uint8_t buf[sizeof(CombinationKey)];
    return new (buf) CombinationKey(code);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_KeyCode(ConsumerControlCode code)
  {
    static uint8_t buf[sizeof(ConsumerControl)];
    return new (buf) ConsumerControl(code);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_KeyCode(SystemControlCode code)
  {
    static uint8_t buf[sizeof(SystemControl)];
    return new (buf) SystemControl(code);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_KeyCode(PointingButtonsCode code)
  {
    static uint8_t buf[sizeof(PointingButton)];
    return new (buf) PointingButton(code);
  }

  //------------------------------------------------------------------+
  // TAP
  //------------------------------------------------------------------+
  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_Tap(NotNullActionPtr action, uint8_t n_times = 1, uint16_t tap_speed_ms = HID_ENGINE_DEFAULT_TAP_SPEED_MS)
  {
    static uint8_t buf[sizeof(Tap)];
    return new (buf) Tap(action, n_times, tap_speed_ms);
  }

  //------------------------------------------------------------------+
  // TAP_WHEN_RELEASED
  //------------------------------------------------------------------+
  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_TapWhenReleased(NotNullActionPtr action, uint8_t n_times = 1, uint16_t tap_speed_ms = HID_ENGINE_DEFAULT_TAP_SPEED_MS)
  {
    static uint8_t buf[sizeof(TapWhenReleased)];
    return new (buf) TapWhenReleased(action, n_times, tap_speed_ms);
  }

  //------------------------------------------------------------------+
  // TAP_DANCE
  //------------------------------------------------------------------+
  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_TapDance(const TapDance::Pair pair0,
                                const TapDance::Pair pair1,
                                uint32_t tapping_term_ms = HID_ENGINE_DEFAULT_TAPPING_TERM_MS)
  {
    static etl::array<TapDance::Pair, 2> pairs{pair0, pair1};
    static etl::span<PointingDeviceId> pointing_device_ids;
    static uint8_t buf[sizeof(TapDance)];
    return new (buf) TapDance(pairs, pointing_device_ids, 0, tapping_term_ms);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_TapDance(const TapDance::Pair pair0,
                                const TapDance::Pair pair1,
                                const TapDance::Pair pair2,
                                uint32_t tapping_term_ms = HID_ENGINE_DEFAULT_TAPPING_TERM_MS)
  {
    static etl::array<TapDance::Pair, 3> pairs{pair0, pair1, pair2};
    static etl::span<PointingDeviceId> pointing_device_ids;
    static uint8_t buf[sizeof(TapDance)];
    return new (buf) TapDance(pairs, pointing_device_ids, 0, tapping_term_ms);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_TapDance(const TapDance::Pair pair0,
                                const TapDance::Pair pair1,
                                const TapDance::Pair pair2,
                                const TapDance::Pair pair3,
                                uint32_t tapping_term_ms = HID_ENGINE_DEFAULT_TAPPING_TERM_MS)
  {
    static etl::array<TapDance::Pair, 4> pairs{pair0, pair1, pair2, pair3};
    static etl::span<PointingDeviceId> pointing_device_ids;
    static uint8_t buf[sizeof(TapDance)];
    return new (buf) TapDance(pairs, pointing_device_ids, 0, tapping_term_ms);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_TapDance(const TapDance::Pair pair0,
                                const TapDance::Pair pair1,
                                const TapDance::Pair pair2,
                                const TapDance::Pair pair3,
                                const TapDance::Pair pair4,
                                uint32_t tapping_term_ms = HID_ENGINE_DEFAULT_TAPPING_TERM_MS)
  {
    static etl::array<TapDance::Pair, 5> pairs{pair0, pair1, pair2, pair3, pair4};
    static etl::span<PointingDeviceId> pointing_device_ids;
    static uint8_t buf[sizeof(TapDance)];
    return new (buf) TapDance(pairs, pointing_device_ids, 0, tapping_term_ms);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_TapDance(const TapDance::Pair pair0,
                                const TapDance::Pair pair1,
                                const TapDance::Pair pair2,
                                const TapDance::Pair pair3,
                                const TapDance::Pair pair4,
                                const TapDance::Pair pair5,
                                uint32_t tapping_term_ms = HID_ENGINE_DEFAULT_TAPPING_TERM_MS)
  {
    static etl::array<TapDance::Pair, 6> pairs{pair0, pair1, pair2, pair3, pair4, pair5};
    static etl::span<PointingDeviceId> pointing_device_ids;
    static uint8_t buf[sizeof(TapDance)];
    return new (buf) TapDance(pairs, pointing_device_ids, 0, tapping_term_ms);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_TapDance(const TapDance::Pair pair0,
                                const TapDance::Pair pair1,
                                const TapDance::Pair pair2,
                                const TapDance::Pair pair3,
                                const TapDance::Pair pair4,
                                const TapDance::Pair pair5,
                                const TapDance::Pair pair6,
                                uint32_t tapping_term_ms = HID_ENGINE_DEFAULT_TAPPING_TERM_MS)
  {
    static etl::array<TapDance::Pair, 7> pairs{pair0, pair1, pair2, pair3, pair4, pair5, pair6};
    static etl::span<PointingDeviceId> pointing_device_ids;
    static uint8_t buf[sizeof(TapDance)];
    return new (buf) TapDance(pairs, pointing_device_ids, 0, tapping_term_ms);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_TapDance(const TapDance::Pair pair0,
                                const TapDance::Pair pair1,
                                const TapDance::Pair pair2,
                                const TapDance::Pair pair3,
                                const TapDance::Pair pair4,
                                const TapDance::Pair pair5,
                                const TapDance::Pair pair6,
                                const TapDance::Pair pair7,
                                uint32_t tapping_term_ms = HID_ENGINE_DEFAULT_TAPPING_TERM_MS)
  {
    static etl::array<TapDance::Pair, 8> pairs{pair0, pair1, pair2, pair3, pair4, pair5, pair6, pair7};
    static etl::span<PointingDeviceId> pointing_device_ids;
    static uint8_t buf[sizeof(TapDance)];
    return new (buf) TapDance(pairs, pointing_device_ids, 0, tapping_term_ms);
  }

  //------------------------------------------------------------------+
  // TAP_DANCE_DECIDE_MOVE_POINTER
  //------------------------------------------------------------------+
  template <uint64_t ID1, uint64_t ID2, size_t N>
  NotNullActionPtr new_TapDanceDecidMovePointer(const TapDance::Pair pair0,
                                                const TapDance::Pair pair1,
                                                const PointingDeviceId (&pointing_device_ids)[N],
                                                uint16_t move_threshold = 0,
                                                uint32_t tapping_term_ms = HID_ENGINE_DEFAULT_TAPPING_TERM_MS)
  {
    static etl::array<TapDance::Pair, 2> pairs{pair0, pair1};
    static etl::array<PointingDeviceId, N> _pointing_device_ids;
    _pointing_device_ids.assign(std::begin(pointing_device_ids), std::end(pointing_device_ids));
    static uint8_t buf[sizeof(TapDance)];
    return new (buf) TapDance(pairs, _pointing_device_ids, move_threshold, tapping_term_ms);
  }

  template <uint64_t ID1, uint64_t ID2, size_t N>
  NotNullActionPtr new_TapDanceDecidMovePointer(const TapDance::Pair pair0,
                                                const TapDance::Pair pair1,
                                                const TapDance::Pair pair2,
                                                const PointingDeviceId (&pointing_device_ids)[N],
                                                uint16_t move_threshold = 0,
                                                uint32_t tapping_term_ms = HID_ENGINE_DEFAULT_TAPPING_TERM_MS)
  {
    static etl::array<TapDance::Pair, 3> pairs{pair0, pair1, pair2};
    static etl::array<PointingDeviceId, N> _pointing_device_ids;
    _pointing_device_ids.assign(std::begin(pointing_device_ids), std::end(pointing_device_ids));
    static uint8_t buf[sizeof(TapDance)];
    return new (buf) TapDance(pairs, _pointing_device_ids, move_threshold, tapping_term_ms);
  }

  template <uint64_t ID1, uint64_t ID2, size_t N>
  NotNullActionPtr new_TapDanceDecidMovePointer(const TapDance::Pair pair0,
                                                const TapDance::Pair pair1,
                                                const TapDance::Pair pair2,
                                                const TapDance::Pair pair3,
                                                const PointingDeviceId (&pointing_device_ids)[N],
                                                uint16_t move_threshold = 0,
                                                uint32_t tapping_term_ms = HID_ENGINE_DEFAULT_TAPPING_TERM_MS)
  {
    static etl::array<TapDance::Pair, 4> pairs{pair0, pair1, pair2, pair3};
    static etl::array<PointingDeviceId, N> _pointing_device_ids;
    _pointing_device_ids.assign(std::begin(pointing_device_ids), std::end(pointing_device_ids));
    static uint8_t buf[sizeof(TapDance)];
    return new (buf) TapDance(pairs, _pointing_device_ids, move_threshold, tapping_term_ms);
  }

  template <uint64_t ID1, uint64_t ID2, size_t N>
  NotNullActionPtr new_TapDanceDecidMovePointer(const TapDance::Pair pair0,
                                                const TapDance::Pair pair1,
                                                const TapDance::Pair pair2,
                                                const TapDance::Pair pair3,
                                                const TapDance::Pair pair4,
                                                const PointingDeviceId (&pointing_device_ids)[N],
                                                uint16_t move_threshold = 0,
                                                uint32_t tapping_term_ms = HID_ENGINE_DEFAULT_TAPPING_TERM_MS)
  {
    static etl::array<TapDance::Pair, 5> pairs{pair0, pair1, pair2, pair3, pair4};
    static etl::array<PointingDeviceId, N> _pointing_device_ids;
    _pointing_device_ids.assign(std::begin(pointing_device_ids), std::end(pointing_device_ids));
    static uint8_t buf[sizeof(TapDance)];
    return new (buf) TapDance(pairs, _pointing_device_ids, move_threshold, tapping_term_ms);
  }

  template <uint64_t ID1, uint64_t ID2, size_t N>
  NotNullActionPtr new_TapDanceDecidMovePointer(const TapDance::Pair pair0,
                                                const TapDance::Pair pair1,
                                                const TapDance::Pair pair2,
                                                const TapDance::Pair pair3,
                                                const TapDance::Pair pair4,
                                                const TapDance::Pair pair5,
                                                const PointingDeviceId (&pointing_device_ids)[N],
                                                uint16_t move_threshold = 0,
                                                uint32_t tapping_term_ms = HID_ENGINE_DEFAULT_TAPPING_TERM_MS)
  {
    static etl::array<TapDance::Pair, 6> pairs{pair0, pair1, pair2, pair3, pair4, pair5};
    static etl::array<PointingDeviceId, N> _pointing_device_ids;
    _pointing_device_ids.assign(std::begin(pointing_device_ids), std::end(pointing_device_ids));
    static uint8_t buf[sizeof(TapDance)];
    return new (buf) TapDance(pairs, _pointing_device_ids, move_threshold, tapping_term_ms);
  }

  template <uint64_t ID1, uint64_t ID2, size_t N>
  NotNullActionPtr new_TapDanceDecidMovePointer(const TapDance::Pair pair0,
                                                const TapDance::Pair pair1,
                                                const TapDance::Pair pair2,
                                                const TapDance::Pair pair3,
                                                const TapDance::Pair pair4,
                                                const TapDance::Pair pair5,
                                                const TapDance::Pair pair6,
                                                const PointingDeviceId (&pointing_device_ids)[N],
                                                uint16_t move_threshold = 0,
                                                uint32_t tapping_term_ms = HID_ENGINE_DEFAULT_TAPPING_TERM_MS)
  {
    static etl::array<TapDance::Pair, 7> pairs{pair0, pair1, pair2, pair3, pair4, pair5, pair6};
    static etl::array<PointingDeviceId, N> _pointing_device_ids;
    _pointing_device_ids.assign(std::begin(pointing_device_ids), std::end(pointing_device_ids));
    static uint8_t buf[sizeof(TapDance)];
    return new (buf) TapDance(pairs, _pointing_device_ids, move_threshold, tapping_term_ms);
  }

  template <uint64_t ID1, uint64_t ID2, size_t N>
  NotNullActionPtr new_TapDanceDecidMovePointer(const TapDance::Pair pair0,
                                                const TapDance::Pair pair1,
                                                const TapDance::Pair pair2,
                                                const TapDance::Pair pair3,
                                                const TapDance::Pair pair4,
                                                const TapDance::Pair pair5,
                                                const TapDance::Pair pair6,
                                                const TapDance::Pair pair7,
                                                const PointingDeviceId (&pointing_device_ids)[N],
                                                uint16_t move_threshold = 0,
                                                uint32_t tapping_term_ms = HID_ENGINE_DEFAULT_TAPPING_TERM_MS)
  {
    static etl::array<TapDance::Pair, 8> pairs{pair0, pair1, pair2, pair3, pair4, pair5, pair6, pair7};
    static etl::array<PointingDeviceId, N> _pointing_device_ids;
    _pointing_device_ids.assign(std::begin(pointing_device_ids), std::end(pointing_device_ids));
    static uint8_t buf[sizeof(TapDance)];
    return new (buf) TapDance(pairs, _pointing_device_ids, move_threshold, tapping_term_ms);
  }

  //------------------------------------------------------------------+
  // HOLD_TAP
  //------------------------------------------------------------------+
  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_HoldTap(NotNullActionPtr hold_action,
                               NotNullActionPtr tap_action,
                               uint32_t tapping_term_ms = HID_ENGINE_DEFAULT_TAPPING_TERM_MS)
  {
    static etl::array<TapDance::Pair, 1> pairs{
        TapDance::Pair{hold_action, tap_action},
    };
    static etl::span<PointingDeviceId> pointing_device_ids;
    static uint8_t buf[sizeof(TapDance)];
    return new (buf) TapDance(pairs, pointing_device_ids, 0, tapping_term_ms);
  }

  //------------------------------------------------------------------+
  // HOLD_TAP_DECIDE_MOVE_POINTER
  //------------------------------------------------------------------+
  template <uint64_t ID1, uint64_t ID2, size_t N>
  NotNullActionPtr new_HoldTapDecideMovePointer(NotNullActionPtr hold_action,
                                                NotNullActionPtr tap_action,
                                                const PointingDeviceId (&pointing_device_ids)[N],
                                                uint16_t move_threshold = 0,
                                                uint32_t tapping_term_ms = HID_ENGINE_DEFAULT_TAPPING_TERM_MS)
  {
    static etl::array<TapDance::Pair, 1> pairs{
        TapDance::Pair{hold_action, tap_action},
    };
    static etl::array<PointingDeviceId, N> _pointing_device_ids;
    _pointing_device_ids.assign(std::begin(pointing_device_ids), std::end(pointing_device_ids));
    static uint8_t buf[sizeof(TapDance)];

    return new (buf) TapDance(pairs, _pointing_device_ids, move_threshold, tapping_term_ms);
  }

  //------------------------------------------------------------------+
  // POINTER_MOVE
  //------------------------------------------------------------------+
  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_PointerMove(int16_t x, int16_t y)
  {
    static uint8_t buf[sizeof(PointerMove)];
    return new (buf) PointerMove(x, y);
  }

  //------------------------------------------------------------------+
  // POINTING_SCROLL
  //------------------------------------------------------------------+
  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_PointingScroll(int8_t scroll, int8_t pan)
  {
    static uint8_t buf[sizeof(PointingScroll)];
    return new (buf) PointingScroll(scroll, pan);
  }

  //------------------------------------------------------------------+
  // CONST_SPEED
  //------------------------------------------------------------------+
  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_ConstSpeed(NotNullActionPtr action, uint32_t interval_ms, uint32_t delay_ms)
  {
    static uint8_t buf[sizeof(ConstSpeed)];
    return new (buf) ConstSpeed(action, interval_ms, delay_ms);
  }

  //------------------------------------------------------------------+
  // IF
  //------------------------------------------------------------------+
  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_If(bool (*func)(), NotNullActionPtr true_action, NotNullActionPtr false_action)
  {
    static uint8_t buf[sizeof(If)];
    return new (buf) If(func, true_action, false_action);
  }

  //------------------------------------------------------------------+
  // MULTI
  //------------------------------------------------------------------+
  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_Multi(const NotNullActionPtr action0,
                             const NotNullActionPtr action1)
  {
    static etl::array<NotNullActionPtr, 2> actions{action0, action1};
    static uint8_t buf[sizeof(Multi)];
    return new (buf) Multi(actions);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_Multi(const NotNullActionPtr action0,
                             const NotNullActionPtr action1,
                             const NotNullActionPtr action2)
  {
    static etl::array<NotNullActionPtr, 3> actions{action0, action1, action2};
    static uint8_t buf[sizeof(Multi)];
    return new (buf) Multi(actions);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_Multi(const NotNullActionPtr action0,
                             const NotNullActionPtr action1,
                             const NotNullActionPtr action2,
                             const NotNullActionPtr action3)
  {
    static etl::array<NotNullActionPtr, 4> actions{action0, action1, action2, action3};
    static uint8_t buf[sizeof(Multi)];
    return new (buf) Multi(actions);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_Multi(const NotNullActionPtr action0,
                             const NotNullActionPtr action1,
                             const NotNullActionPtr action2,
                             const NotNullActionPtr action3,
                             const NotNullActionPtr action4)
  {
    static etl::array<NotNullActionPtr, 5> actions{action0, action1, action2, action3, action4};
    static uint8_t buf[sizeof(Multi)];
    return new (buf) Multi(actions);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_Multi(const NotNullActionPtr action0,
                             const NotNullActionPtr action1,
                             const NotNullActionPtr action2,
                             const NotNullActionPtr action3,
                             const NotNullActionPtr action4,
                             const NotNullActionPtr action5)
  {
    static etl::array<NotNullActionPtr, 6> actions{action0, action1, action2, action3, action4, action5};
    static uint8_t buf[sizeof(Multi)];
    return new (buf) Multi(actions);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_Multi(const NotNullActionPtr action0,
                             const NotNullActionPtr action1,
                             const NotNullActionPtr action2,
                             const NotNullActionPtr action3,
                             const NotNullActionPtr action4,
                             const NotNullActionPtr action5,
                             const NotNullActionPtr action6)
  {
    static etl::array<NotNullActionPtr, 7> actions{action0, action1, action2, action3, action4, action5, action6};
    static uint8_t buf[sizeof(Multi)];
    return new (buf) Multi(actions);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_Multi(const NotNullActionPtr action0,
                             const NotNullActionPtr action1,
                             const NotNullActionPtr action2,
                             const NotNullActionPtr action3,
                             const NotNullActionPtr action4,
                             const NotNullActionPtr action5,
                             const NotNullActionPtr action6,
                             const NotNullActionPtr action7)
  {
    static etl::array<NotNullActionPtr, 8> actions{action0, action1, action2, action3, action4, action5, action6, action7};
    static uint8_t buf[sizeof(Multi)];
    return new (buf) Multi(actions);
  }

  //------------------------------------------------------------------+
  // TOGGLE
  //------------------------------------------------------------------+
  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_Toggle(NotNullActionPtr action)
  {
    static uint8_t buf[sizeof(Toggle)];
    return new (buf) Toggle(action);
  }

  //------------------------------------------------------------------+
  // REPEAT
  //------------------------------------------------------------------+
  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_Repeat(NotNullActionPtr action, uint32_t delay_ms, uint32_t interval_ms)
  {
    static uint8_t buf[sizeof(Repeat)];
    return new (buf) Repeat(action, delay_ms, interval_ms);
  }

  //------------------------------------------------------------------+
  // NOP
  //------------------------------------------------------------------+
  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_NoOperation()
  {
    static uint8_t buf[sizeof(NoOperation)];
    return new (buf) NoOperation();
  }

  //------------------------------------------------------------------+
  // MAP_SHIFT
  //------------------------------------------------------------------+
  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_MapShift(MapShiftId map_shift_id)
  {
    static etl::array<MapShiftIdLink, 1> map_shift_id_links{
        MapShift::IdToLink(map_shift_id),
    };
    static uint8_t buf[sizeof(MapShift)];
    return new (buf) MapShift(map_shift_id_links);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_MapShift(MapShiftId map_shift_id0,
                                MapShiftId map_shift_id1)
  {
    static etl::array<MapShiftIdLink, 2> map_shift_id_links{
        MapShift::IdToLink(map_shift_id0),
        MapShift::IdToLink(map_shift_id1),
    };
    static uint8_t buf[sizeof(MapShift)];
    return new (buf) MapShift(map_shift_id_links);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_MapShift(MapShiftId map_shift_id0,
                                MapShiftId map_shift_id1,
                                MapShiftId map_shift_id2)
  {
    static etl::array<MapShiftIdLink, 3> map_shift_id_links{
        MapShift::IdToLink(map_shift_id0),
        MapShift::IdToLink(map_shift_id1),
        MapShift::IdToLink(map_shift_id2),
    };
    static uint8_t buf[sizeof(MapShift)];
    return new (buf) MapShift(map_shift_id_links);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_MapShift(MapShiftId map_shift_id0,
                                MapShiftId map_shift_id1,
                                MapShiftId map_shift_id2,
                                MapShiftId map_shift_id3)
  {
    static etl::array<MapShiftIdLink, 4> map_shift_id_links{
        MapShift::IdToLink(map_shift_id0),
        MapShift::IdToLink(map_shift_id1),
        MapShift::IdToLink(map_shift_id2),
        MapShift::IdToLink(map_shift_id3),
    };
    static uint8_t buf[sizeof(MapShift)];
    return new (buf) MapShift(map_shift_id_links);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_MapShift(MapShiftId map_shift_id0,
                                MapShiftId map_shift_id1,
                                MapShiftId map_shift_id2,
                                MapShiftId map_shift_id3,
                                MapShiftId map_shift_id4)
  {
    static etl::array<MapShiftIdLink, 5> map_shift_id_links{
        MapShift::IdToLink(map_shift_id0),
        MapShift::IdToLink(map_shift_id1),
        MapShift::IdToLink(map_shift_id2),
        MapShift::IdToLink(map_shift_id3),
        MapShift::IdToLink(map_shift_id4),
    };
    static uint8_t buf[sizeof(MapShift)];
    return new (buf) MapShift(map_shift_id_links);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_MapShift(MapShiftId map_shift_id0,
                                MapShiftId map_shift_id1,
                                MapShiftId map_shift_id2,
                                MapShiftId map_shift_id3,
                                MapShiftId map_shift_id4,
                                MapShiftId map_shift_id5)
  {
    static etl::array<MapShiftIdLink, 6> map_shift_id_links{
        MapShift::IdToLink(map_shift_id0),
        MapShift::IdToLink(map_shift_id1),
        MapShift::IdToLink(map_shift_id2),
        MapShift::IdToLink(map_shift_id3),
        MapShift::IdToLink(map_shift_id4),
        MapShift::IdToLink(map_shift_id5),
    };
    static uint8_t buf[sizeof(MapShift)];
    return new (buf) MapShift(map_shift_id_links);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_MapShift(MapShiftId map_shift_id0,
                                MapShiftId map_shift_id1,
                                MapShiftId map_shift_id2,
                                MapShiftId map_shift_id3,
                                MapShiftId map_shift_id4,
                                MapShiftId map_shift_id5,
                                MapShiftId map_shift_id6)
  {
    static etl::array<MapShiftIdLink, 7> map_shift_id_links{
        MapShift::IdToLink(map_shift_id0),
        MapShift::IdToLink(map_shift_id1),
        MapShift::IdToLink(map_shift_id2),
        MapShift::IdToLink(map_shift_id3),
        MapShift::IdToLink(map_shift_id4),
        MapShift::IdToLink(map_shift_id5),
        MapShift::IdToLink(map_shift_id6),
    };
    static uint8_t buf[sizeof(MapShift)];
    return new (buf) MapShift(map_shift_id_links);
  }

  template <uint64_t ID1, uint64_t ID2>
  NotNullActionPtr new_MapShift(MapShiftId map_shift_id0,
                                MapShiftId map_shift_id1,
                                MapShiftId map_shift_id2,
                                MapShiftId map_shift_id3,
                                MapShiftId map_shift_id4,
                                MapShiftId map_shift_id5,
                                MapShiftId map_shift_id6,
                                MapShiftId map_shift_id7)
  {
    static etl::array<MapShiftIdLink, 8> map_shift_id_links{
        MapShift::IdToLink(map_shift_id0),
        MapShift::IdToLink(map_shift_id1),
        MapShift::IdToLink(map_shift_id2),
        MapShift::IdToLink(map_shift_id3),
        MapShift::IdToLink(map_shift_id4),
        MapShift::IdToLink(map_shift_id5),
        MapShift::IdToLink(map_shift_id6),
        MapShift::IdToLink(map_shift_id7),
    };
    static uint8_t buf[sizeof(MapShift)];
    return new (buf) MapShift(map_shift_id_links);
  }

  //------------------------------------------------------------------+
  // KEYMAP
  //------------------------------------------------------------------+
  template <uint64_t ID1, uint64_t ID2, size_t N>
  etl::span<Key> KEYMAP_impl(const Key (&keymap)[N])
  {
    static etl::vector<Key, N> result;
    result.assign(std::begin(keymap), std::end(keymap));
    return result;
  }

}
