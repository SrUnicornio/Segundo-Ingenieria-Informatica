#ifndef __EXPLORATIONFUNTION_H__
#define __EXPLORATIONFUNTION_H__

#include "../includespath.h"
#include "../DispersionFuntion/DispecionFuntion.h"

// Abstract base class for exploration functions
template <typename Key>
class ExplorationFunction
{
public:
  virtual unsigned operator()(const Key &, unsigned) const = 0;
  virtual ~ExplorationFunction() {}
};

// Concrete class for linear exploration function
template <typename Key>
class LinearExploration : public ExplorationFunction<Key>
{
public:
  virtual unsigned operator()(const Key &, unsigned i) const override
  {
    return i;
  }
};

// Concrete class for quadratic exploration function
template <typename Key>
class QuadraticExploration : public ExplorationFunction<Key>
{
public:
  virtual unsigned operator()(const Key &, unsigned i) const override
  {
    return i * i;
  }
};

// Concrete class for double hash exploration function
template <typename Key>
class DoubleHashExploration : public ExplorationFunction<Key>
{
private:
  DispersionFunction<Key> *f_;

public:
  DoubleHashExploration(DispersionFunction<Key> *dispersion) : f_(dispersion) {}
  virtual unsigned operator()(const Key &k, unsigned i) const override
  {
    return (*f_)(k)*i;
  }
};

// Concrete class for rehashing exploration function
template <typename Key>
class RehashingExploration : public ExplorationFunction<Key>
{
private:
  DispersionFunction<Key> *f_;

public:
  RehashingExploration(DispersionFunction<Key> *dispersion) : f_(dispersion) {}
  virtual unsigned operator()(const Key &k, unsigned i) const override
  {
    unsigned index = (*f_)(k);
    for (unsigned j = 0; j < i; ++j)
    {
      index = (*f_)(k);
    }
    return index;
  }
};

#endif // __EXPLORATIONFUNTION_H__
