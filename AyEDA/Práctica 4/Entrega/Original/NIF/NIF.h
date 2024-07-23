#ifndef __NIF_H__
#define __NIF_H__

#include "../includespath.h"

class NIF
{
public:
  NIF() : value_(rand() % 100000000), leter_('A' + rand() % 26) {}
  NIF(long val, char leter) : value_(val), leter_(leter) {}
  bool operator==(const NIF &other) const
  {
    return value_ == other.value_;
  }
  bool operator!=(const NIF &other) const
  {
    return !(value_ == other.value_);
  }
  operator long() const { return value_; }

  long getValue() const { return value_; }
  char getLeter() const { return leter_; }

  friend std::istream &operator>>(std::istream &is, NIF &nif)
  {
    std::cout << "\nEnter NIF value: ";
    is >> nif.value_;
    std::cout << "Enter NIF letter: ";
    is >> nif.leter_;
    return is;
  }
  friend std::ostream &operator<<(std::ostream &os, const NIF &nif)
  {
    os << nif.value_ << nif.leter_;
    return os;
  }

private:
  long value_;
  char leter_;
};

#endif // __NIF_H__
