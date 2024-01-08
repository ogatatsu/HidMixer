
#include "HidMixer/HidEngineDetail/ActionTapper.h"
#include "HidMixer/HidEngineDetail/HidEngineTask.h"

namespace HidMixer::HidEngineDetail
{

  etl::list<ActionTapperClass::Data, HID_ENGINE_ACTION_TAPPER_QUEUE_SIZE> ActionTapperClass::_queue;
  ActionTapperClass::Data ActionTapperClass::_running = {.action = nullptr, .num_of_taps = 0, .tap_speed_ms = 0};
  ActionTapperClass::State ActionTapperClass::_state = ActionTapperClass::State::NotRunning;
  TimerHandle_t ActionTapperClass::_timer_handle = nullptr;
  StaticTimer_t ActionTapperClass::_timer_buffer;

  void ActionTapperClass::init()
  {
    _timer_handle = xTimerCreateStatic("ActionTapper", 1, pdFALSE, nullptr, timer_callback, &_timer_buffer);
  }

  bool ActionTapperClass::tap(Action *action, uint8_t n_times, uint16_t tap_speed_ms)
  {
    if (n_times == 0 || action == nullptr)
    {
      return true;
    }

    // 動いてないならタップしてタップ回数が残ってるならタイマーを動かす
    if (_state == State::NotRunning)
    {
      uint8_t actual_n_times = action->tap(n_times);
      uint8_t num_of_taps = std::max(n_times - actual_n_times, 0);

      if (num_of_taps != 0)
      {
        _state = State::WaitTimer;
        _running.action = action;
        _running.num_of_taps = num_of_taps;
        _running.tap_speed_ms = tap_speed_ms;
        xTimerChangePeriod(_timer_handle, pdMS_TO_TICKS(tap_speed_ms), portMAX_DELAY);
      }

      return true;
    }

    // キューが空で今動いてるActionと同じならタップ回数を足す
    if (_queue.empty() && _running.action == action && _running.tap_speed_ms == tap_speed_ms)
    {
      _running.num_of_taps = std::min(_running.num_of_taps + n_times, UINT8_MAX);
      return true;
    }

    // キューが空でないならキューの最後のActionと比較して同じならタップ回数を足す
    if (_queue.empty() == false)
    {
      Data &last = _queue.back();

      if (last.action == action && last.tap_speed_ms == tap_speed_ms)
      {
        last.num_of_taps = std::min(last.num_of_taps + n_times, UINT8_MAX);
        return true;
      }
    }

    // キューに空きがあるなら追加
    if (_queue.available())
    {
      Data data = {
          .action = action,
          .num_of_taps = n_times,
          .tap_speed_ms = tap_speed_ms,
      };
      _queue.push_back(data);
      return true;
    }

    return false;
  }

  void ActionTapperClass::onTimer()
  {
    uint8_t actual_n_times = _running.action->tap(_running.num_of_taps);
    _running.num_of_taps = std::max(_running.num_of_taps - actual_n_times, 0);

    // まだタップ回数が残っている場合同じActionで再度タップ
    if (_running.num_of_taps > 0)
    {
      _state = State::WaitTimer;
      xTimerStart(_timer_handle, portMAX_DELAY);
    }
    // キューが空でないなら次のActionの準備
    else if (_queue.empty() == false)
    {
      _running = _queue.front();
      _queue.pop_front();
      xTimerChangePeriod(_timer_handle, pdMS_TO_TICKS(_running.tap_speed_ms), portMAX_DELAY);
    }
    // 動作終了
    else
    {
      _state = State::NotRunning;
      _running.action = nullptr;
      _running.num_of_taps = 0;
      _running.tap_speed_ms = 0;
    }
  }

  void ActionTapperClass::timer_callback(TimerHandle_t timerHandle)
  {
    HidEngineTask.enqueEvent(ActionTapperEventData{});
  }

  ActionTapperClass ActionTapper;

}
