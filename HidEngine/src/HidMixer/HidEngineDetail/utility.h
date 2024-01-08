
#pragma once

#include "FreeRTOS.h"
#include "task.h"

namespace HidMixer::HidEngineDetail
{

  inline uint32_t millis()
  {
    return xTaskGetTickCount() * 1000ULL / configTICK_RATE_HZ;
  }

  template <typename TList, typename Tlink>
  bool erase_link_from_list(TList &list, Tlink *link)
  {
    for (auto itr = list.begin(); itr != list.end(); itr++)
    {
      if (itr->etl_next == link->etl_next && itr->etl_previous == link->etl_previous)
      {
        list.erase(itr);
        return true;
      }
    }
    return false;
  }

  template <typename TList, typename Tlink>
  bool erase_link_from_list(TList &list, Tlink &link)
  {
    for (auto itr = list.begin(); itr != list.end(); itr++)
    {
      if (itr->etl_next == link.etl_next && itr->etl_previous == link.etl_previous)
      {
        list.erase(itr);
        return true;
      }
    }
    return false;
  }

}
