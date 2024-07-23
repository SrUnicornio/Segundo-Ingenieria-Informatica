#include <cstdlib>
#include <ctime>
#include <iostream>
#include <optional>
#include <vector>

static unsigned long compare_swapkeys_count = 0;

template <class Key>
class DispersionFunction
{
public:
  DispersionFunction() = default;
  DispersionFunction(unsigned tableSize) : tableSize_(tableSize) {}
  ~DispersionFunction() {}
  unsigned GetTableSize() const { return tableSize_; }
  virtual unsigned operator()(const Key &key) const = 0;

protected:
  unsigned tableSize_;
};

template <class Key>
class Modulo : public DispersionFunction<Key>
{
public:
  Modulo() = default;
  Modulo(unsigned tableSize) : DispersionFunction<Key>(tableSize) {}
  ~Modulo() {}

  unsigned operator()(const Key &key) const override
  {
    return key % tableSize_;
  }
};

template <class Key>
class Suma : public DispersionFunction<Key>
{
public:
  Suma() = default;
  Suma(unsigned tableSize) : DispersionFunction<Key>(tableSize) {}
  ~Suma() {}

  unsigned operator()(const Key &key) const override
  {
    unsigned sum = 0;
    long n = key;
    while (n > 0)
    {
      sum += n % 10;
      n /= 10;
    }
    return sum % tableSize_;
  }
};

template <class Key>
class PseudoAzar : public DispersionFunction<Key>
{
public:
  PseudoAzar() = default;
  PseudoAzar(unsigned tableSize) : DispersionFunction<Key>(tableSize) {}
  ~PseudoAzar() {}

  unsigned operator()(const Key &key) const override
  {
    srand(key);
    return rand() % this->GetTableSize();
  }
};

template <class Key>
class ExplorationFunction
{
public:
  ExplorationFunction() = default;
  ExplorationFunction(unsigned tableSize) : tableSize_(tableSize) {}
  ~ExplorationFunction() {}
  unsigned GetTableSize() const { return tableSize_; }
  virtual unsigned operator()(const Key &key, unsigned i) const = 0;

protected:
  unsigned tableSize_;
};

template <class Key>
class Lineal : public ExplorationFunction<Key>
{
public:
  Lineal() = default;
  Lineal(unsigned tableSize) : ExplorationFunction<Key>(tableSize) {}
  ~Lineal() {}

  unsigned operator()(const Key &key, unsigned i) const override
  {
    return i;
  }
};

template <class Key>
class Square : public ExplorationFunction<Key>
{
public:
  Square() = default;
  Square(unsigned tableSize) : ExplorationFunction<Key>(tableSize) {}
  ~Square() {}

  unsigned operator()(const Key &key, unsigned i) const override
  {
    return i * i;
  }
};

template <class Key>
class DoubleHashing : public ExplorationFunction<Key>
{
public:
  DoubleHashing() = default;
  DoubleHashing(const DispersionFunction<Key> &f) : f_(f) {}
  ~DoubleHashing() {}

  unsigned operator()(const Key &key, unsigned i) const override
  {
    return (f_(key) * i);
  }

private:
  const DispersionFunction<Key> &f_;
};

template <class Key>
class ReHashing : public ExplorationFunction<Key>
{
public:
  ReHashing() = default;
  ReHashing(const DispersionFunction<Key> &f) : f_(f) {}
  ~ReHashing() {}

  unsigned operator()(const Key &key, unsigned i) const override
  {
    unsigned result = f_(key);
    for (unsigned count = 0; count < i; count++)
    {
      result = f_(result);
    }
    return result;
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
  bool search(const Key &key) const override
  {
    for (const auto &k : elements_)
    {
      if (k == key)
      {
        compare_swapkeys_count++;
        return true;
      }
    }
    return false;
  }
  bool insert(const Key &key) override
  {
    if (search(key))
    {
      return false;
    }
    compare_swapkeys_count++;
    elements_.push_back(key);
    return true;
  }

private:
  std::vector<Key> elements_;
};

template <class Key>
class StaticSequence : public Sequence<Key>
{
public:
  StaticSequence(unsigned blockSize) : blockSize_(blockSize) {}
  bool search(const Key &key) const override
  {
    for (const auto &k : elements_)
    {
      if (k == key)
      {
        compare_swapkeys_count++;
        return true;
      }
    }
    return false;
  }
  bool insert(const Key &key) override
  {
    if (search(key))
    {
      return false;
    }
    if (elements_.size() < blockSize_)
    {
      compare_swapkeys_count++;
      elements_.push_back(key);
      return true;
    }
    return false;
  }
  bool isFull() const { return elements_.size() == blockSize_; }

private:
  unsigned blockSize_;
  std::vector<Key> elements_;
};

template <class Key, class Container = StaticSequence<Key>>
class HashTable : public Sequence<Key>
{
private:
  unsigned tableSize;
  std::vector<Container *> table;
  DispersionFunction<Key> &fd;
  ExplorationFunction<Key> &fe;
  unsigned blockSize;

public:
  HashTable(unsigned size, DispersionFunction<Key> &dispersionFunc, ExplorationFunction<Key> &explorationFunc, unsigned block)
      : tableSize(size), fd(dispersionFunc), fe(explorationFunc), blockSize(block)
  {
    table.resize(tableSize);
    for (unsigned i = 0; i < tableSize; ++i)
    {
      table[i] = new Container(blockSize);
    }
  }
  ~HashTable()
  {
    for (unsigned i = 0; i < tableSize; ++i)
    {
      delete table[i];
    }
    table.clear();
  }

  bool search(const Key &key) const override
  {
    unsigned hashvalue = fd(key);
    unsigned index = hashvalue % tableSize;
    unsigned i = 0;
    while (i < tableSize)
    {
      if (table[index]->search(key))
      {
        return true;
      }
      index = (hashvalue + fe(key, i)) % tableSize;
      i++;
    }
    return false;
  }
  bool insert(const Key &key) override
  {
    unsigned hashValue = fd(key);
    unsigned index = hashValue % tableSize;

    if (table[index]->isFull())
    {
      // Si el contenedor está lleno, intentar reasignar con exploración
      unsigned i = 1;
      while (i < tableSize)
      {
        unsigned newIndex = (index + fe(key, i)) % tableSize;
        if (!table[newIndex]->isFull())
        {
          index = newIndex;
          break;
        }
        ++i;
      }

      // Si no se encuentra un índice disponible, no se puede insertar
      if (i == tableSize)
        return false;
    }

    // Insertar en el contenedor correspondiente
    return table[index]->insert(key);
  }
};

template <class Key>
class HashTable<Key, DynamicSequence<Key>> : public Sequence<Key>
{
private:
  unsigned tableSize;
  std::vector<DynamicSequence<Key>> table;
  DispersionFunction<Key> &fd;

public:
  HashTable(unsigned size, DispersionFunction<Key> &dispersionFunc)
      : tableSize(size), fd(dispersionFunc)
  {
    table.resize(tableSize);
  }
  ~HashTable() {}

  bool search(const Key &key) const override
  {
    unsigned hashvalue = fd(key);
    unsigned index = hashvalue % tableSize;
    return table[index].search(key);
  }
  bool insert(const Key &key) override
  {
    unsigned hashValue = fd(key);
    unsigned index = hashValue % tableSize;
    return table[index].insert(key);   
  }
};

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

  friend std::istream &operator>>(std::istream &is, NIF &nif)
  {
    is >> nif.value_;
    return is;
  }

private:
  long value_;
  char leter_;
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
  std::cout << "Usage: ./HashTable -ts <s> -fd <f> -hash <open|close> -bs <s> -fe <f>\n";
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

int main(int argc, char *argv[])
{
  // srand(time(NULL));
  // std::cout << "Hello, World!" << std::endl;
  NIF nif = NIF();
  std::cout << "NIF: " << nif << std::endl;

  // Prueba de Modulo
  {
    Modulo<NIF> modulo(10);
    std::cout << "Modulo: " << modulo(nif) << std::endl;
  }

  // Prueba de Suma
  {
    Suma<NIF> suma(10);
    std::cout << "Suma: " << suma(nif) << std::endl;
  }

  // Prueba de PseudoAzar
  {
    PseudoAzar<NIF> pseudoAzar(20);
    std::cout << "PseudoAzar: " << pseudoAzar(nif) << std::endl;
  }

  // Prueba de Lineal
  {
    Lineal<NIF> lineal(10);
    std::cout << "Lineal: " << lineal(nif, 5) << std::endl;
  }

  // Prueba de Square
  {
    Square<NIF> square(10);
    std::cout << "Square: " << square(nif, 5) << std::endl;
  }

  // Prueba de DoubleHashing
  {
    Modulo<NIF> modulo(10);
    DoubleHashing<NIF> doubleHashing(modulo);
    std::cout << "DoubleHashing: " << doubleHashing(nif, 5) << std::endl;
  }

  // Prueba de ReHashing
  {
    Modulo<NIF> modulo(10);
    ReHashing<NIF> reHashing(modulo);
    std::cout << "ReHashing: " << reHashing(nif, 5) << std::endl;
  }

  // Prueba de DynamicSequence
  {
    NIF nif2 = NIF();
    DynamicSequence<NIF> dynamicSequence;
    dynamicSequence.insert(nif);
    std::cout << "DynamicSequence search 1: " << (dynamicSequence.search(nif) ? "true" : "false") << std::endl;
    std::cout << "DynamicSequence search 2: " << (dynamicSequence.search(nif2) ? "true" : "false") << std::endl;
  }

  // Prueba de StaticSequence
  {
    NIF nif2 = NIF();
    std::cout << "NIF2: " << nif2 << std::endl;
    StaticSequence<NIF> staticSequence(10);
    staticSequence.insert(nif);
    std::cout << "StaticSequence search 1: " << (staticSequence.search(nif) ? "true" : "false") << std::endl;
    std::cout << "StaticSequence search 2: " << (staticSequence.search(nif2) ? "true" : "false") << std::endl;
  }

  // Prueba de parse_args
  {
    // imprimir en rojo Prueba de parse_args
    std::cout << "\033[1;31mPrueba de parse_args\033[0m" << std::endl;
    const char *args[] = {"./HashTable", "-ts", "10", "-fd", "sum", "-hash", "open", "-bs", "4", "-fe", "linear"};
    auto options = parse_args(11, args);
    if (options.has_value())
    {
      std::cout << "Table size: " << options.value().tableSize << std::endl;
      std::cout << "Dispersion function: " << options.value().dispersionFunction << std::endl;
      std::cout << "Hash type: " << options.value().hashType << std::endl;
      std::cout << "Block size: " << options.value().blockSize << std::endl;
      std::cout << "Exploration function: " << options.value().explorationFunction << std::endl;
    }
    else
    {
      std::cout << "Invalid arguments" << std::endl;
    }
      

    Modulo<NIF> modulo(10);
    Suma<NIF> suma(10);
    PseudoAzar<NIF> pseudoAzar(20);
    Lineal<NIF> lineal(10);
    Square<NIF> square(10);
    DoubleHashing<NIF> doubleHashing(modulo);
    ReHashing<NIF> reHashing(modulo);

    HashTable<NIF, StaticSequence<NIF>> hashTable1(1, suma, doubleHashing, 1);
    HashTable<NIF, DynamicSequence<NIF>> hashTable2(1, modulo);

    // Insertar elementos en las tablas hash
    while (std::cin >> nif)
    {
      if (nif.getValue() == 0)
      {
        break;
      }
      hashTable1.insert(nif);
      hashTable2.insert(nif);
    }

    // Buscar elementos en las tablas hash
    while (std::cin >> nif)
    {
      if (nif.getValue() == 0)
      {
        break;
      }
      std::cout << "Buscando " << nif << " en hashTable1: " << (hashTable1.search(nif) ? "true" : "false") << std::endl;
      std::cout << "Buscando " << nif << " en hashTable2: " << (hashTable2.search(nif) ? "true" : "false") << std::endl;
    }
  }

  std::cout << "compare_swapkeys_count: " << compare_swapkeys_count << std::endl;
  return 0;
}
