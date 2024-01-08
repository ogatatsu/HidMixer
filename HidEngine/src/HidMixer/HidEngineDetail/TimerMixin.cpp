
#include "HidMixer/HidEngineDetail/TimerMixin.h"
#include "HidMixer/HidEngineDetail/HidEngineTask.h"

namespace HidMixer::HidEngineDetail
{
  StaticTimer_t TimerMixin::_timer_buffers[HID_ENGINE_TIMER_MIXIN_MAX_TIMER_COUNT];
  TimerMixin::Data TimerMixin::_data_buffers[HID_ENGINE_TIMER_MIXIN_MAX_TIMER_COUNT];
  etl::forward_list<TimerMixin::Data *, HID_ENGINE_TIMER_MIXIN_MAX_TIMER_COUNT> TimerMixin::_pool;

  void TimerMixin::timer_callback(TimerHandle_t timer_handle)
  {
    Data *data = static_cast<TimerMixin::Data *>(pvTimerGetTimerID(timer_handle));

    HidEngineTask.enqueEvent(TimerEventData{data->cls, data->timer_number});

    // プールに戻す
    data->cls = nullptr;
    data->timer_number = 0;
    _pool.push_front(data);
  }

  void TimerMixin::init()
  {
    for (size_t i = 0; i < HID_ENGINE_TIMER_MIXIN_MAX_TIMER_COUNT; i++)
    {
      _data_buffers[i].cls = nullptr;
      _data_buffers[i].timer_number = 0;
      _data_buffers[i].timer_handle = xTimerCreateStatic("TimerMixin", 1, pdFALSE, &(_data_buffers[i]), timer_callback, &(_timer_buffers[i]));
      _pool.push_front(&(_data_buffers[i]));
    }
  }

  TimerMixin::TimerMixin() : _is_active(false), _num_of_timer(0)
  {
  }

  bool TimerMixin::startTimer(unsigned int ms)
  {
    if (_pool.empty())
    {
      return false;
    }

    Data *data = _pool.front();
    _pool.pop_front();

    data->cls = this;
    data->timer_number = ++_num_of_timer;

    xTimerChangePeriod(data->timer_handle, pdMS_TO_TICKS(ms), portMAX_DELAY);
    _is_active = true;

    return true;
  }

  void TimerMixin::stopTimer()
  {
    _is_active = false;
  }

  bool TimerMixin::isTimerActive()
  {
    return _is_active;
  }

  // プリエンプティブなマルチタスクだとタスク切換えとstartTimer関数の呼び出しタイミングによっては
  // 2個以上のタイマーイベントがイベントキューに入るかもしれないが
  // timer_numberを見て最後に作ったイベントのみを実行する(使用側からはワンショットタイマーが１個だけ動いてるように見える)
  void TimerMixin::trigger(unsigned int timer_number)
  {
    if (_is_active && (_num_of_timer == timer_number))
    {
      _is_active = false;
      onTimer();
    }
  }

}
