
#include "HidMixer/HidEngineDetail/types.h"
#include "utility.h"

namespace HidMixer::HidEngineDetail
{
  //------------------------------------------------------------------+
  // Action
  //------------------------------------------------------------------+
  Action::Action() : _is_pressed(false)
  {
  }

  void Action::press()
  {
    if (_is_pressed == false)
    {
      _is_pressed = true;
      onPress();
    }
  }

  void Action::release()
  {
    if (_is_pressed)
    {
      _is_pressed = false;
      onRelease();
    }
  }

  uint8_t Action::tap(uint8_t n_times)
  {
    if (_is_pressed)
    {
      _is_pressed = false;
      onRelease();
      return 1;
    }

    return onTap(n_times);
  }

  //------------------------------------------------------------------+
  // BeforeOtherKeyPressEventListener
  //------------------------------------------------------------------+

  BeforeOtherKeyPressEventListener::List BeforeOtherKeyPressEventListener::_listener_list;

  BeforeOtherKeyPressEventListener::BeforeOtherKeyPressEventListener(Action *action)
      : BeforeOtherKeyPressEventListenerLink(),
        _action(action)
  {
  }

  bool BeforeOtherKeyPressEventListener::startListenBeforeOtherKeyPress()
  {
    if (is_linked())
    {
      return false;
    }

    _listener_list.push_back(*this);
    return true;
  }

  bool BeforeOtherKeyPressEventListener::stopListenBeforeOtherKeyPress()
  {
    return erase_link_from_list(_listener_list, this);
  }

  void BeforeOtherKeyPressEventListener::_notifyBeforeOtherKeyPress(uint8_t key_id)
  {
    BeforeOtherKeyPressEventListener *copyed_itr[_listener_list.size()];
    size_t i = 0;
    for (auto &listener : _listener_list)
    {
      copyed_itr[i++] = &listener;
    }

    for (auto listener : copyed_itr)
    {
      if (listener->_action->getKeyId() != key_id)
      {
        listener->onBeforeOtherKeyPress(key_id);
      }
    }
  }

  //------------------------------------------------------------------+
  // BeforePointerMoveEventListener
  //------------------------------------------------------------------+

  BeforePointerMoveEventListener::List BeforePointerMoveEventListener::_listener_list;

  BeforePointerMoveEventListener::BeforePointerMoveEventListener()
      : BeforePointerMoveEventListenerLink()
  {
  }

  bool BeforePointerMoveEventListener::startListenBeforePointerMove()
  {
    if (is_linked())
    {
      return false;
    }

    _listener_list.push_back(*this);
    return true;
  }

  bool BeforePointerMoveEventListener::stopListenBeforePointerMove()
  {
    return erase_link_from_list(_listener_list, this);
  }

  void BeforePointerMoveEventListener::_notifyBeforePointerMove(PointingDeviceId pointing_device_id, int16_t delta_x, int16_t delta_y)
  {
    BeforePointerMoveEventListener *copyed_itr[_listener_list.size()];
    size_t i = 0;
    for (auto &listener : _listener_list)
    {
      copyed_itr[i++] = &listener;
    }

    for (auto listener : copyed_itr)
    {
      listener->onBeforePointerMove(pointing_device_id, delta_x, delta_y);
    }
  }

  //------------------------------------------------------------------+
  // BeforeEncoderRotateEventListener
  //------------------------------------------------------------------+

  BeforeEncoderRotateEventListener::List BeforeEncoderRotateEventListener::_listener_list;

  BeforeEncoderRotateEventListener::BeforeEncoderRotateEventListener()
      : BeforeEncoderRotateEventListenerLink()
  {
  }

  bool BeforeEncoderRotateEventListener::startListenBeforeEncoderRotate()
  {
    if (is_linked())
    {
      return false;
    }

    _listener_list.push_back(*this);
    return true;
  }

  bool BeforeEncoderRotateEventListener::stopListenBeforeEncoderRotate()
  {
    return erase_link_from_list(_listener_list, this);
  }

  void BeforeEncoderRotateEventListener::_notifyBeforeEncoderRotate(EncoderId encoder_id, int16_t step)
  {
    BeforeEncoderRotateEventListener *copyed_itr[_listener_list.size()];
    size_t i = 0;
    for (auto &listener : _listener_list)
    {
      copyed_itr[i++] = &listener;
    }

    for (auto listener : copyed_itr)
    {
      listener->onBeforeRotateEncoder(encoder_id, step);
    }
  }

  //------------------------------------------------------------------+
  // BeforeGestureEventListener
  //------------------------------------------------------------------+
  BeforeGestureEventListener::List BeforeGestureEventListener::_listener_list;

  BeforeGestureEventListener::BeforeGestureEventListener()
      : BeforeGestureEventListenerLink()
  {
  }

  bool BeforeGestureEventListener::startListenBeforeGesture()
  {
    if (is_linked())
    {
      return false;
    }

    _listener_list.push_back(*this);
    return true;
  }

  bool BeforeGestureEventListener::stopListenBeforeGesture()
  {
    return erase_link_from_list(_listener_list, this);
  }

  void BeforeGestureEventListener::_notifyBeforeGesture(GestureId gesture_id, PointingDeviceId pointing_device_id)
  {
    BeforeGestureEventListener *copyed_itr[_listener_list.size()];
    size_t i = 0;
    for (auto &listener : _listener_list)
    {
      copyed_itr[i++] = &listener;
    }

    for (auto listener : copyed_itr)
    {
      listener->onBeforeGesture(gesture_id, pointing_device_id);
    }
  }

}
