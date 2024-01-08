
#pragma once

#include <stddef.h>
#include <stdint.h>

namespace HidMixer
{

  // uint8_t(0~255の値)が入るSet
  class Set
  {
    friend bool operator==(const Set &a, const Set &b);
    friend Set operator|(const Set &a, const Set &b);
    friend Set operator-(const Set &a, const Set &b);

  public:
    Set();

    bool insert(uint8_t val);
    void insertAll(const uint8_t vals[], size_t len);
    Set &operator|=(const Set &rhs);

    bool remove(uint8_t val);
    void removeAll(const uint8_t vals[], size_t len);
    Set &operator-=(const Set &rhs);

    bool update(uint8_t val, bool b);

    void clear();

    bool contains(uint8_t val) const;
    bool containsAll(const uint8_t vals[], size_t len) const;
    bool containsAny(const uint8_t vals[], size_t len) const;

    void toArray(uint8_t buf[]) const;
    uint16_t count() const;

  private:
    // 32 * 8 = 256
    uint32_t _data[8];
    uint16_t _count;

    static const size_t _data_size = 8;
    static const size_t _data_bit_size = 32;
  };

  // 比較
  bool operator==(const Set &a, const Set &b);
  bool operator!=(const Set &a, const Set &b);
  // 和集合
  Set operator|(const Set &a, const Set &b);
  // 差集合
  Set operator-(const Set &a, const Set &b);

}
