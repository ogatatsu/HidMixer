
#pragma once

#include "FreeRTOS.h"
#include "HidMixer/HidEngineDetail/HidEngine_config.h"
#include "etl/forward_list.h"
#include "timers.h"

namespace HidMixer::HidEngineDetail
{

  class HidEngineClass;
  class HidEngineTaskClass;

  class TimerMixin
  {
    friend class HidEngineClass;
    friend class HidEngineTaskClass;

  protected:
    TimerMixin();
    bool startTimer(unsigned int ms);
    void stopTimer();
    bool isTimerActive();
    virtual void onTimer() {}

  private:
    struct Data
    {
      TimerMixin *cls;
      unsigned int timer_number;
      TimerHandle_t timer_handle;
    };

    void trigger(unsigned int timer_number);

    static void init();
    static void timer_callback(TimerHandle_t timer_handle);

    bool _is_active;
    unsigned int _num_of_timer;

    static StaticTimer_t _timer_buffers[HID_ENGINE_TIMER_MIXIN_MAX_TIMER_COUNT];
    static Data _data_buffers[HID_ENGINE_TIMER_MIXIN_MAX_TIMER_COUNT];
    static etl::forward_list<Data *, HID_ENGINE_TIMER_MIXIN_MAX_TIMER_COUNT> _pool;
  };

}
