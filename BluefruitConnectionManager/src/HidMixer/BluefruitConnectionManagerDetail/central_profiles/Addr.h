
#pragma once

#include "HidMixer/MemStore.h"

namespace HidMixer::BluefruitConnectionManagerDetail
{
  struct Addr
  {
    Addr() : _addr()
    {
    }

    Addr(uint8_t addr[6])
    {
      set(addr);
    }

    bool isSameAddr(uint8_t addr[6])
    {
      if (memcmp(_addr, addr, 6) == 0)
      {
        return true;
      }
      return false;
    }

    void set(uint8_t addr[6])
    {
      memcpy(_addr, addr, 6);
    }

  private:
    uint8_t _addr[6];
  };

  extern MemStore AddrStore;
}
