
#pragma once

#include "HidMixer/HidEngineDetail/HidEngine.h"
#include "etl/variant.h"
#include "queue.h"

namespace HidMixer::HidEngineDetail
{

  struct ApplyToKeymapEventData
  {
    Set key_ids;
  };

  struct MovePointerEventData
  {
    PointingDeviceId pointing_device_id;
  };

  struct RotateEncoderEventData
  {
    EncoderId encoder_id;
  };

  struct TimerEventData
  {
    TimerMixin *cls;
    unsigned int timer_number;
  };

  struct ActionTapperEventData
  {
    // empty
  };

  using EventData = etl::variant<ApplyToKeymapEventData,
                                 MovePointerEventData,
                                 RotateEncoderEventData,
                                 TimerEventData,
                                 ActionTapperEventData>;

  class HidEngineTaskClass
  {
  public:
#ifdef configUSE_CORE_AFFINITY
    static void start(UBaseType_t task_priority, UBaseType_t core_affinity_mask);
#else
    static void start(UBaseType_t task_priority);
#endif
    static void enqueEvent(const EventData &evt);

  private:
    static void task(void *pvParameters);

    static TaskHandle_t _task_handle;
    static StackType_t _task_stack[HID_ENGINE_TASK_STACK_SIZE];
    static StaticTask_t _task_tcb;
    static QueueHandle_t _event_queue;
    static uint8_t _event_queue_storage[HID_ENGINE_EVENT_QUEUE_SIZE * sizeof(EventData)];
    static StaticQueue_t _event_queue_struct;
  };

  extern HidEngineTaskClass HidEngineTask;

}
