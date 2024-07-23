#ifndef __DISPERSIONFUNCTION_H__
#define __DISPERSIONFUNCTION_H__

#include "../includespath.h"

// Abstract base class for dispersion functions
template <typename Key>
class DispersionFunction
{
public:
  virtual unsigned operator()(const Key &) const = 0;
  virtual ~DispersionFunction() {}
};

// Concrete class for modulo dispersion function
template <typename Key>
class ModuloDispersion : public DispersionFunction<Key>
{
private:
  unsigned tableSize_;

public:
  ModuloDispersion(unsigned size) : tableSize_(size) {}
  virtual unsigned operator()(const Key &k) const override
  {
    return atoi(k.getISBN().c_str()) % tableSize_;
  }
};

// Concrete class for sum dispersion function
template <typename Key>
class SumDispersion : public DispersionFunction<Key>
{
private:
  unsigned tableSize_;

public:
  SumDispersion(unsigned size) : tableSize_(size) {}
  virtual unsigned operator()(const Key &k) const override
  {
    // Assuming Key is iterable
    unsigned sum = 0;
    unsigned value = atoi(k.getISBN().c_str());
    while (value > 0)
    {
      sum += value % 10;
      value /= 10;
    }
    return sum % tableSize_;
  }
};

// Concrete class for pseudo-random dispersion function
template <typename Key>
class PseudoRandomDispersion : public DispersionFunction<Key>
{
private:
  unsigned tableSize_;

public:
  PseudoRandomDispersion(unsigned size) : tableSize_(size) {}
  virtual unsigned operator()(const Key &k) const override
  {
    srand(atoi(k.getISBN().c_str()));
    return rand() % tableSize_;
  }
};

#endif // __DISPERSIONFUNCTION_H__