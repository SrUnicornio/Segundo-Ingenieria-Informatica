#include <array>
#include <cstdlib>
#include <iostream>
#include <list>
#include <optional>
#include <string>
#include <vector>

template <class Key>
class DispersionFunction
{
public:
  DispersionFunction(unsigned int size) : tableSize_(size) {}
  virtual unsigned operator()(const Key &key) const = 0;
  virtual ~DispersionFunction() {}
  unsigned getTableSize() const { return tableSize_; }

protected:
  unsigned tableSize_;
};

template <class Key>
class ModuleDispersion : public DispersionFunction<Key>
{
public:
  ModuleDispersion(unsigned int size) : DispersionFunction<Key>(size) {}
  unsigned operator()(const Key &key) const override
  {
    return key % this->getTableSize();
  }
};

template <class Key>
class PseudorandomDispersion : public DispersionFunction<Key>
{
public:
  PseudorandomDispersion(unsigned int size) : DispersionFunction<Key>(size) {}
  unsigned operator()(const Key &key) const override
  {
    srand(key);
    return rand() % this->getTableSize();
  }
};

template <class Key>
class SumDispersion : public DispersionFunction<Key>
{
public:
  SumDispersion(unsigned int size) : DispersionFunction<Key>(size) {}
  unsigned operator()(const Key &key) const override
  {
    unsigned sum{0};
    for (const auto &val : key)
    {
      sum += val;
    }
    return sum % this->getTableSize();
  }
};

template <class Key>
class ExplorationFunction
{
public:
  virtual unsigned operator()(const Key &, unsigned i) const = 0;
  virtual ~ExplorationFunction() {}
};

template <class Key>
class DoubleHashingExploration : public ExplorationFunction<Key>
{
public:
  DoubleHashingExploration(const DispersionFunction<Key> &f) : f_(f) {}
  unsigned operator()(const Key &key, unsigned i) const override
  {
    return (f_(key) * i);
  }

private:
  const DispersionFunction<Key> &f_;
};

template <class Key>
class LinearExploration : public ExplorationFunction<Key>
{
public:
  unsigned operator()(const Key &key, unsigned i) const override
  {
    return i;
  }
};

template <class Key>
class QuadraticExploration : public ExplorationFunction<Key>
{
public:
  unsigned operator()(const Key &key, unsigned i) const override
  {
    return i * i;
  }
};

template <class Key>
class RehashingExploration : public ExplorationFunction<Key>
{
public:
  RehashingExploration(const DispersionFunction<Key> &f) : f_(f) {}
  unsigned operator()(const Key &key, unsigned i) const override
  {
    unsigned hashValue = f_(key);
    for (unsigned count = 0; count < i; count++)
    {
      hashValue = f_(hashValue);
    }
    return hashValue;
  }

private:
  const DispersionFunction<Key> &f_;
};

template <class Key>
class Sequence
{
public:
  virtual bool search(const Key &key) const = 0;
  virtual bool insert(const Key &key) = 0;
  virtual ~Sequence() {}
};

template <class Key>
class DynamicSequence : public Sequence<Key>
{
public:
  DynamicSequence() {}
  bool search(const Key &key) const override;
  bool insert(const Key &key) override;

private:
  std::vector<Key> elements_;
};

template <class Key>
bool DynamicSequence<Key>::search(const Key &key) const
{
  for (const auto &k : elements_)
  {
    if (k == key)
    {
      return true;
    }
  }
  return false;
}

template <class Key>
bool DynamicSequence<Key>::insert(const Key &key)
{
  if (!search(key))
  {
    elements_.push_back(key);
    return true;
  }
  return false;
}

template <class Key>
class StaticSequence : public Sequence<Key>
{
public:
  StaticSequence(size_t tSize, DispersionFunction<Key> &fd, ExplorationFunction<Key> &fe, size_t bSize)
      : tableSize_(tSize), fd_(fd), fe_(fe), blockSize_(bSize)
  {
    table_ = new Key[tableSize_];
    for (size_t i = 0; i < tableSize_; ++i)
    {
      table_[i] = Key{};
    }
  }
  bool search(const Key &key) const override;
  bool insert(const Key &key) override;
  bool isFull() const;

private:
  size_t tableSize_;
  Key *table_;
  DispersionFunction<Key> &fd_;
  ExplorationFunction<Key> &fe_;
  size_t blockSize_;
};

template <class Key>
bool StaticSequence<Key>::search(const Key &key) const
{
  unsigned hash = fd_(key);
  for (size_t i = 0; i < tableSize_; ++i)
  {
    unsigned index = (hash + fe_(key, i)) % tableSize_;
    if (table_[index] == key)
    {
      return true;
    }
  }
  return false;
}

template <class Key>
bool StaticSequence<Key>::insert(const Key &key)
{
  unsigned hash = fd_(key);
  for (size_t i = 0; i < tableSize_; ++i)
  {
    unsigned index = (hash + fe_(key, i)) % tableSize_;
    if (table_[index] == Key{}) // Assuming Key{} gives a default value for Key
    {
      table_[index] = key;
      return true;
    }
  }
  return false;
}

template <class Key, class Container = StaticSequence<Key>>
class HashTable : public Sequence<Key>
{
public:
  HashTable(unsigned sz, DispersionFunction<Key> &fd, ExplorationFunction<Key> &fe, unsigned bsz)
      : tableSize_(sz), fd_(fd), fe_(fe), blockSize_(bsz)
  {
    table_ = new Container(sz, fd, fe, bsz);
  }
  HashTable(unsigned sz, DispersionFunction < Key >> &fd) : tableSize_(sz), fd_(fd)
  {
    table_ = new Container[tableSize_];
  }

private:
  size_t tableSize_;
  Container *table_;
  DispersionFunction<Key> &fd_;
  ExplorationFunction<Key> &fe_;
  size_t blockSize_;
};

class NIF
{
public:
  NIF() : value_(rand() % 100000000) {}
  NIF(long int val) : value_(val) {}
  bool operator==(const NIF &other) const { return value_ == other.value_; }
  bool operator!=(const NIF &other) const { return !(*this == other); }
  operator long() const { return value_; }

private:
  long int value_;
};

struct program_options
{
  unsigned tableSize;
  std::string dispersionFunction;
  std::string hashType;
  unsigned blockSize;
  std::string explorationFunction;
};

void Usage()
{
  std::cout << "Usage: ./hashTable -ts <s> -fd <f> -hash <open|close> -bs <s> -fe <f>\n";
  std::cout << "-ts <s> : table size\n";
  std::cout << "-fd <f> : dispersion function -> sum, random, or mod\n";
  std::cout << "-hash <open|close> : collision resolution technique\n";
  std::cout << "-bs <s> : block size for closed addressing\n";
  std::cout << "-fe <f> : exploration function for closed addressing -> linear, quadratic, or double\n";
}

std::optional<program_options> parse_args(int argc, const char *const argv[])
{
  program_options options;
  std::vector<std::string> args(argv + 1, argv + argc);
  if (args.empty())
  {
    return std::nullopt;
  }

  for (auto it = args.begin(); it != args.end(); ++it)
  {
    if (*it == "-ts")
    {
      if (it + 1 >= args.end())
      {
        return std::nullopt;
      }
      options.tableSize = std::stoi(*(it + 1));
      it += 1;
    }
    else if (*it == "-fd")
    {
      if (it + 1 >= args.end())
      {
        return std::nullopt;
      }
      options.dispersionFunction = *(it + 1);
      it += 1;
    }
    else if (*it == "-hash")
    {
      if (it + 1 >= args.end())
      {
        return std::nullopt;
      }
      options.hashType = *(it + 1);
      it += 1;
    }
    else if (*it == "-bs")
    {
      if (it + 1 >= args.end())
      {
        return std::nullopt;
      }
      options.blockSize = std::stoi(*(it + 1));
      it += 1;
    }
    else if (*it == "-fe")
    {
      if (it + 1 >= args.end())
      {
        return std::nullopt;
      }
      options.explorationFunction = *(it + 1);
      it += 1;
    }
  }

  return options;
}

int main(int argc, const char *const argv[])
{
  auto options = parse_args(argc, argv);
  if (!options)
  {
    Usage();
    return 1;
  }

  std::cout << "Table size: " << options->tableSize << std::endl;
  std::cout << "Dispersion function: " << options->dispersionFunction << std::endl;
  std::cout << "Collision resolution technique: " << options->hashType << std::endl;
  std::cout << "Block size: " << options->blockSize << std::endl;
  std::cout << "Exploration function: " << options->explorationFunction << std::endl;

  return 0;
}
