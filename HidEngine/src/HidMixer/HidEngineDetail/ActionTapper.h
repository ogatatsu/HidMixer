
#pragma once

#include "FreeRTOS.h"
#include "HidMixer/HidEngineDetail/HidEngine.h"
#include "HidMixer/HidEngineDetail/HidEngine_config.h"
#include "etl/list.h"
#include "timers.h"

namespace HidMixer::HidEngineDetail
{

  class ActionTapperClass
  {
    friend class HidEngineClass;
    friend class HidEngineTaskClass;

  public:
    static bool tap(Action *action, uint8_t n_times = 1, uint16_t tap_speed_ms = HID_ENGINE_DEFAULT_TAP_SPEED_MS);

  private:
    static void init();
    static void onTimer();
    static void timer_callback(TimerHandle_t timer_handle);

    struct Data
    {
      Action *action;
      uint8_t num_of_taps;
      uint16_t tap_speed_ms;
    };

    enum class State
    {
      NotRunning,
      WaitTimer,
      ChangeActionInTheNext,
    };

    static etl::list<Data, HID_ENGINE_ACTION_TAPPER_QUEUE_SIZE> _queue;
    static Data _running;
    static State _state;
    static TimerHandle_t _timer_handle;
    static StaticTimer_t _timer_buffer;
  };

  extern ActionTapperClass ActionTapper;

}
