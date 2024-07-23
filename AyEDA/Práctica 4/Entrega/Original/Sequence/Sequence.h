#ifndef __SEQUENCE_H__
#define __SEQUENCE_H__

#include "../includespath.h"
static unsigned long compare_swapkeys_count = 0;

// Abstract base class for sequences
template <class Key>
class Sequence
{
public:
  virtual bool search(const Key &k) const = 0;
  virtual bool insert(const Key &k) = 0;
  virtual ~Sequence() {}
};

// Dynamic sequence for open addressing
template <class Key>
class dynamicSequence : public Sequence<Key>
{
private:
  std::vector<Key> data;

public:
  bool search(const Key &k) const override
  {
    for (const auto &key : data)
    {
      if (key == k)
        compare_swapkeys_count++;
        return true;
    }
    return false;
  }

  bool insert(const Key &k) override
  {
    if (!search(k))
    {
      compare_swapkeys_count++;
      data.push_back(k);
      return true;
    }
    return false;
  }
};

// Static sequence for closed addressing
template <class Key>
class staticSequence : public Sequence<Key>
{
private:
  std::vector<Key> data;
  unsigned blockSize;

public:
  staticSequence(unsigned size) : blockSize(size)
  {
    data.reserve(blockSize);
  }

  bool search(const Key &k) const override
  {
    for (const auto &key : data)
    {
      if (key == k)
        compare_swapkeys_count++;
        return true;
    }
    return false;
  }

  bool insert(const Key &k) override
  {
    if (data.size() < blockSize && !search(k))
    {
      data.push_back(k);
      return true;
    }
    return false;
  }

  bool isFull() const
  {
    compare_swapkeys_count++;
    return data.size() >= blockSize;
  }
};

#endif // __SEQUENCE_H__