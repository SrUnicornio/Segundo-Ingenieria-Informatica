#include <iostream>
// para rand
#include <stdlib.h>
#include <vector>

template <class Key>
class FuncionDispersion
{
public:
  virtual unsigned operator()(const Key &k) const = 0;
};

template <class Key>
class fdModulo : public FuncionDispersion<Key>
{
public:
  fdModulo(const unsigned n) : tableSize(n) {}
  unsigned operator()(const Key &k) const
  {
    return k % tableSize;
  }

private:
  unsigned tableSize;
};

template <class Key>
class fdSuma : public FuncionDispersion<Key>
{
public:
  fdSuma(const unsigned n) : tableSize(n) {}
  unsigned operator()(const Key &k) const
  {
    unsigned d = 0, y = 0;
    unsigned x = k;
    while (x > 0)
    {
      y = x % 10;
      d += y;
      x = x / 10;
    }
    return (d % tableSize);
  }

private:
  unsigned tableSize;
};

template <class Key>
class fdPseudoaleatoria : public FuncionDispersion<Key>
{
public:
  fdPseudoaleatoria(const unsigned n) : tableSize(n) {}
  unsigned operator()(const Key &k) const
  {
    int x = k;
    srand(x);                  // generamos semilla para poder generar numeros
    return rand() % tableSize; // para generar un número entre 0 y tableSize
  }

private:
  unsigned tableSize;
};

template <class Key>
class FuncionExploracion
{
public:
  virtual unsigned operator()(const Key &k, unsigned i) const = 0;
};

template <class Key>
class feLineal : public FuncionExploracion<Key>
{
public:
  unsigned operator()(const Key &k, unsigned i) const
  {
    return i;
  }
};

template <class Key>
class feCuadratica : public FuncionExploracion<Key>
{
public:
  unsigned operator()(const Key &k, unsigned i) const
  {
    return (i * i);
  }
};

template <class Key>
class feDobleDispersion : public FuncionExploracion<Key>
{
public:
  feDobleDispersion(FuncionDispersion<Key> *fd)
  {
    fd_ = fd;
  }
  unsigned operator()(const Key &k, unsigned i) const { return i * fd_(k); }

private:
  FuncionDispersion<Key> *fd_;
};

template <class Key>
class feRedispersion : public FuncionExploracion<Key>
{
public:
  feRedispersion(FuncionDispersion<Key> *fd)
  {
    fd_ = fd;
  }
  unsigned operator()(const Key &k, unsigned i) const
  {
    return i * (1 + fd_(k));
  }

private:
  FuncionDispersion<Key> *fd_;
};

template <class Key>
class Sequence
{
public:
  Sequence(){};
  ~Sequence(){};
  virtual bool Search(const Key &k) const = 0;
  virtual bool Insert(const Key &k) = 0;
  virtual bool isFull() const = 0;
};

template <class Key>
class DynamicSequence : public Sequence<Key>
{
public:
  DynamicSequence(){};
  ~DynamicSequence(){};

  bool Search(const Key &k) const
  {
    for (unsigned i = 0; i < vDatos_.size(); i++)
    {
      if (k == vDatos_[i])
      {
        return true;
      }
    }
    return false;
  }
  bool Insert(const Key &k)
  {
    if (Search(k) == true)
    {
      return false;
    }
    vDatos_.push_back(k);
    return true;
  }

private:
  vector<Key> vDatos_;
};

template <class Key>
class StaticSequence : public Sequence<Key>
{
public:
  StaticSequence(unsigned max_size)
  {
    maxsize_ = max_size;
    vDatos_.reserve(maxsize_);
  };
  ~StaticSequence(){};
  bool Search(const Key &k) const
  {
    for (unsigned i = 0; i < vDatos_.size(); i++)
    {
      if (k == vDatos_[i])
      {
        return true;
      }
    }
    return false;
  }
  bool Insert(const Key &k)
  {
    if (Search(k) || isFull())
    {
      return false;
    }
    vDatos_.push_back(k);
    return true;
  }
  bool isFull() const
  {
    return vDatos_.size() == maxsize_;
  }

private:
  vector<Key> vDatos_;
  unsigned maxsize_;
};

template <class Key, class Container = StaticSequence<Key>>
class HashTable
{

};