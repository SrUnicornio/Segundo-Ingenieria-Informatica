#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include "../DispersionFuntion/DispecionFuntion.h"
#include "../EsplorationFuntion/ExplorationFuntion.h"
#include "../Sequence/Sequence.h"
#include "../includespath.h"

// HashTable class
template <class Key, class Container = staticSequence<Key>>
class HashTable : public Sequence<Key>
{
private:
  unsigned tableSize;
  Container **table;
  DispersionFunction<Key> &fd;
  ExplorationFunction<Key> &fe;
  unsigned blockSize;

public:
  HashTable(unsigned size, DispersionFunction<Key> &dispersion, ExplorationFunction<Key> &exploration, unsigned block = 0)
      : tableSize(size), fd(dispersion), fe(exploration), blockSize(block)
  {
    table = new Container *[tableSize];
    for (unsigned i = 0; i < tableSize; ++i)
    {
      table[i] = new Container(blockSize);
    }
  }

  // Destructor
  ~HashTable()
  {
    for (unsigned i = 0; i < tableSize; ++i)
    {
      delete table[i];
    }
    delete[] table;
  }

  bool search(const Key &k) const override
  {
    unsigned index = fd(k) % tableSize;
    return table[index]->search(k);
  }

  bool insert(const Key &k) override
  {
    unsigned index = fd(k) % tableSize;
    return table[index]->insert(k);
  }

  bool isFull() const
  {
    for (unsigned i = 0; i < tableSize; ++i)
    {
      if (!table[i]->isFull())
        return false;
    }
    return true;
  }

  Key GetKey(const Key &k) const
  {
    return k;
  }

  long GetIndex(const Key &k) const
  {
    for (unsigned i = 0; i < tableSize; ++i)
    {
      if (table[i]->search(k))
        return i;
    }
    return -1;
  }
};

// Specialization for dynamicSequence
template <class Key>
class HashTable<Key, dynamicSequence<Key>> : public Sequence<Key>
{
private:
  unsigned tableSize;
  dynamicSequence<Key> **table;
  DispersionFunction<Key> &fd;

public:
  HashTable(unsigned size, DispersionFunction<Key> &dispersion)
      : tableSize(size), fd(dispersion)
  {
    table = new dynamicSequence<Key> *[tableSize];
    for (unsigned i = 0; i < tableSize; ++i)
    {
      table[i] = new dynamicSequence<Key>;
    }
  }

  // Destructor
  ~HashTable()
  {
    for (unsigned i = 0; i < tableSize; ++i)
    {
      delete table[i];
    }
    delete[] table;
  }

  bool search(const Key &k) const override
  {
    unsigned index = fd(k) % tableSize;
    return table[index]->search(k);
  }

  bool insert(const Key &k) override
  {
    unsigned index = fd(k) % tableSize;
    return table[index]->insert(k);
  }

  Key GetKey(const Key &k) const
  {
    return k;
  }

  long GetIndex(const Key &k) const
  {
    return fd(k) % tableSize;
  }
};

#endif // __HASHTABLE_H__
