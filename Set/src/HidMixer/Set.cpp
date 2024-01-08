
#include "HidMixer/Set.h"
#include <string.h>

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)

#define bitSet(value, bit) ((value) |= (1UL << (bit)))

#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))

namespace HidMixer
{

  Set::Set() : _data(), _count(0)
  {
  }

  bool Set::insert(uint8_t val)
  {
    if (contains(val))
    {
      return false;
    }

    bitSet(_data[val / _data_bit_size], val % _data_bit_size);
    _count++;
    return true;
  }

  void Set::insertAll(const uint8_t vals[], size_t len)
  {
    for (size_t i = 0; i < len; i++)
    {
      insert(vals[i]);
    }
  }

  Set &Set::operator|=(const Set &rhs)
  {
    _count = 0;

    for (size_t i = 0; i < _data_size; i++)
    {
      _data[i] |= rhs._data[i];
      _count += __builtin_popcountl(_data[i]);
    }
    return *this;
  }

  bool Set::remove(uint8_t val)
  {
    if (contains(val) == false)
    {
      return false;
    }

    bitClear(_data[val / _data_bit_size], val % _data_bit_size);
    _count--;
    return true;
  }

  void Set::removeAll(const uint8_t vals[], size_t len)
  {
    for (size_t i = 0; i < len; i++)
    {
      remove(vals[i]);
    }
  }

  Set &Set::operator-=(const Set &rhs)
  {
    _count = 0;
    for (size_t i = 0; i < _data_size; i++)
    {
      _data[i] &= ~(rhs._data[i]);
      _count += __builtin_popcountl(_data[i]);
    }
    return *this;
  }

  bool Set::update(uint8_t val, bool b)
  {
    if (b)
    {
      return insert(val);
    }
    else
    {
      return remove(val);
    }
  }

  void Set::clear()
  {
    memset(_data, 0, sizeof(_data));
    _count = 0;
  }

  bool Set::contains(uint8_t val) const
  {
    return bitRead(_data[val / _data_bit_size], val % _data_bit_size);
  }

  bool Set::containsAll(const uint8_t vals[], size_t len) const
  {
    for (size_t i = 0; i < len; i++)
    {
      if (contains(vals[i]) == false)
      {
        return false;
      }
    }
    return true;
  }

  bool Set::containsAny(const uint8_t vals[], size_t len) const
  {
    for (size_t i = 0; i < len; i++)
    {
      if (contains(vals[i]))
      {
        return true;
      }
    }
    return false;
  }

  void Set::toArray(uint8_t buf[]) const
  {
    if (_count == 0)
      return;

    uint16_t buf_cnt = 0;

    for (size_t i = 0; i < _data_size; i++)
    {
      if (_data[i] == 0)
      {
        continue;
      }
      for (size_t j = 0; j < _data_bit_size; j++)
      {
        uint8_t val = i * _data_bit_size + j;
        if (contains(val))
        {
          buf[buf_cnt++] = val;

          if (buf_cnt == _count)
          {
            return;
          }
        }
      }
    }
  }

  uint16_t Set::count() const
  {
    return _count;
  }

  bool operator==(const Set &a, const Set &b)
  {
    if (a.count() != b.count())
    {
      return false;
    }
    return memcmp(a._data, b._data, sizeof(Set::_data)) ? false : true;
  }

  bool operator!=(const Set &a, const Set &b)
  {
    return !(a == b);
  }

  Set operator|(const Set &a, const Set &b)
  {
    Set result;

    for (size_t i = 0; i < Set::_data_size; i++)
    {
      result._data[i] |= a._data[i];
      result._data[i] |= b._data[i];
      result._count += __builtin_popcountl(result._data[i]);
    }
    return result;
  }

  Set operator-(const Set &a, const Set &b)
  {
    Set result;

    for (size_t i = 0; i < Set::_data_size; i++)
    {
      result._data[i] |= a._data[i];
      result._data[i] &= ~(b._data[i]);
      result._count += __builtin_popcountl(result._data[i]);
    }
    return result;
  }

}
