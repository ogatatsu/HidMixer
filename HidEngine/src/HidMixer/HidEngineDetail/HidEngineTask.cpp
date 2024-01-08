
#include "HidMixer/HidEngineDetail/HidEngineTask.h"
#include "HidMixer/HidEngineDetail/ActionTapper.h"

namespace HidMixer::HidEngineDetail
{

  TaskHandle_t HidEngineTaskClass::_task_handle = nullptr;
  StackType_t HidEngineTaskClass::_task_stack[HID_ENGINE_TASK_STACK_SIZE];
  StaticTask_t HidEngineTaskClass::_task_tcb;
  QueueHandle_t HidEngineTaskClass::_event_queue = nullptr;
  uint8_t HidEngineTaskClass::_event_queue_storage[HID_ENGINE_EVENT_QUEUE_SIZE * sizeof(EventData)];
  StaticQueue_t HidEngineTaskClass::_event_queue_struct;

#ifdef configUSE_CORE_AFFINITY
  void HidEngineTaskClass::start(UBaseType_t task_priority, UBaseType_t core_affinity_mask)
  {
    _event_queue = xQueueCreateStatic(HID_ENGINE_EVENT_QUEUE_SIZE, sizeof(EventData), _event_queue_storage, &_event_queue_struct);
    _task_handle = xTaskCreateStaticAffinitySet(task, "HidEngine", HID_ENGINE_TASK_STACK_SIZE, nullptr, task_priority, _task_stack, &_task_tcb, core_affinity_mask);
  }
#else
  void HidEngineTaskClass::start(UBaseType_t task_priority)
  {
    _event_queue = xQueueCreateStatic(HID_ENGINE_EVENT_QUEUE_SIZE, sizeof(EventData), _event_queue_storage, &_event_queue_struct);
    _task_handle = xTaskCreateStatic(task, "HidEngine", HID_ENGINE_TASK_STACK_SIZE, nullptr, task_priority, _task_stack, &_task_tcb);
  }
#endif

  void HidEngineTaskClass::enqueEvent(const EventData &evt)
  {
    xQueueSend(_event_queue, &evt, portMAX_DELAY);
  }

  void HidEngineTaskClass::task(void *pvParameters)
  {
    while (true)
    {
      EventData evt;
      xQueueReceive(_event_queue, &evt, portMAX_DELAY);

      if (auto *e = etl::get_if<ApplyToKeymapEventData>(&evt))
      {
        HidEngine.applyToKeymap_body(e->key_ids);
      }
      else if (auto *e = etl::get_if<MovePointerEventData>(&evt))
      {
        HidEngine.movePointer_body(e->pointing_device_id);
      }
      else if (auto *e = etl::get_if<RotateEncoderEventData>(&evt))
      {
        HidEngine.rotateEncoder_body(e->encoder_id);
      }
      else if (auto *e = etl::get_if<TimerEventData>(&evt))
      {
        e->cls->trigger(e->timer_number);
      }
      else if (etl::holds_alternative<ActionTapperEventData>(evt))
      {
        ActionTapper.onTimer();
      }
    }
  }

  HidEngineTaskClass HidEngineTask;

}
