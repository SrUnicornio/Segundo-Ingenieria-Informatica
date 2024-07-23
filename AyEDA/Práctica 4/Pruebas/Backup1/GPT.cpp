#include <cstdlib> // For srand() and rand()
#include <ctime>   // For time()
#include <iostream>
#include <vector>
#include <optional>

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
    return k % tableSize_;
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
    unsigned value = k;
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
    srand(k);
    return rand() % tableSize_;
  }
};

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
  virtual bool search(const Key &k) const override
  {
    for (const auto &key : data)
    {
      if (key == k)
        return true;
    }
    return false;
  }

  virtual bool insert(const Key &k) override
  {
    if (!search(k))
    {
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

  virtual bool search(const Key &k) const override
  {
    for (const auto &key : data)
    {
      if (key == k)
        return true;
    }
    return false;
  }

  virtual bool insert(const Key &k) override
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
    return data.size() >= blockSize;
  }
};

// HashTable class
template <class Key, class Container = staticSequence<Key>>
class HashTable: public Sequence<Key>
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
    return fd(k) % tableSize;
  }
};

// Specialization for dynamicSequence
template <class Key>
class HashTable<Key, dynamicSequence<Key>>: public Sequence<Key>
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
  std::cout << "-fe <f> : exploration function for closed addressing -> linear, quadratic, double, or rehashing\n";
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

void Pruebas()
{
  // Example usage
  unsigned tableSize = 2;
  ModuloDispersion<NIF> modulo(tableSize);
  SumDispersion<NIF> sum(tableSize);
  PseudoRandomDispersion<NIF> random(tableSize);

  LinearExploration<NIF> linear;
  QuadraticExploration<NIF> quadratic;
  // Example usage of double hash exploration
  DoubleHashExploration<NIF> doubleHash(&sum);
  // Example usage of rehashing exploration
  RehashingExploration<NIF> rehashing(&sum);

  // Usage example:
  NIF key = NIF(12345678, 'A');
  unsigned index = modulo(key);
  std::cout << "Index for key " << key << " using modulo: " << index << std::endl;
  index = sum(key);
  std::cout << "Index for key " << key << " using sum: " << index << std::endl;
  index = random(key);
  std::cout << "Index for key " << key << " using random: " << index << std::endl;

  unsigned i = 3;
  index = linear(key, i);
  std::cout << "Index for key " << key << " using linear exploration: " << index << std::endl;
  index = quadratic(key, i);
  std::cout << "Index for key " << key << " using quadratic exploration: " << index << std::endl;
  index = doubleHash(key, i);
  std::cout << "Index for key " << key << " using double hash exploration: " << index << std::endl;
  index = rehashing(key, i);
  std::cout << "Index for key " << key << " using rehashing exploration: " << index << std::endl;

  // Example usage
  dynamicSequence<NIF> dynamicSeq;
  staticSequence<NIF> staticSeq(4);

  // Inserting elements into dynamic sequence
  dynamicSeq.insert(NIF(5, 'A'));
  dynamicSeq.insert(NIF(10, 'B'));
  dynamicSeq.insert(NIF(15, 'C'));
  std::cout << "Number 20 is in dynamic sequence? " << (dynamicSeq.search(NIF(20, 'D')) ? "Yes" : "No") << std::endl;
  std::cout << "Number 25 is in dynamic sequence? " << (dynamicSeq.search(NIF(5, 'A')) ? "Yes" : "No") << std::endl;

  // Inserting elements NIFo static sequence
  // Inserting elements into static sequence
  staticSeq.insert(NIF(44444444, 'D'));
  staticSeq.insert(NIF(55555555, 'E'));
  staticSeq.insert(NIF(66666666, 'F'));
  staticSeq.insert(NIF(77777777, 'G'));
  staticSeq.insert(NIF(88888888, 'H')); // This should fail as the sequence is full

  std::cout << "Number 66666666 is in static sequence? " << (staticSeq.search(NIF(66666666, 'F')) ? "Yes" : "No") << std::endl;
  std::cout << "Number 88888888 is in static sequence? " << (staticSeq.search(NIF(88888888, 'H')) ? "Yes" : "No") << std::endl;

  std::cout << "Is static sequence full? " << (staticSeq.isFull() ? "Yes" : "No") << std::endl;

  // Example usage
  tableSize = 2;
  modulo = ModuloDispersion<NIF>(tableSize);

  // Using HashTable with staticSequence (default)
  HashTable<NIF> staticTable(tableSize, modulo, linear, 4);

  // Using HashTable with dynamicSequence
  HashTable<NIF, dynamicSequence<NIF>> dynamicTable(tableSize, modulo);

  staticTable.insert(NIF(42, 'A'));
  dynamicTable.insert(NIF(42, 'A'));

  std::cout << "Search in static table: " << (staticTable.search(NIF(42, 'A')) ? "Found" : "Not found") << std::endl;
  std::cout << "Search in dynamic table: " << (dynamicTable.search(NIF(42, 'A')) ? "Found" : "Not found") << std::endl;

  std::cout << "Is static table full? " << (staticTable.isFull() ? "Yes" : "No") << std::endl;

  NIF value;

  std::cout << "Input value to insert in static table: ";
  while (std::cin >> value && value.getValue() != -1)
  {
    if (staticTable.isFull())
    {
      std::cout << "Static table is full" << std::endl;
      break;
    }
    std::cout << "Insert in static table: " << (staticTable.insert(value) ? "Success" : "Failed") << std::endl;
  }
  std::cout << "Input value to search in static table: ";
  while (std::cin >> value && value.getValue() != -1)
  {
    std::cout << "Search in static table: " << (staticTable.search(value) ? "Found" : "Not found") << std::endl;
    std::cout << "Index for key " << staticTable.GetKey(value) << " is: " << staticTable.GetIndex(value) << std::endl;
  }
  std::cout << "Input value to insert in dynamic table: ";
  while (std::cin >> value && value.getValue() != -1)
  {
    std::cout << "Insert in dynamic table: " << (dynamicTable.insert(value) ? "Success" : "Failed") << std::endl;
  }
  std::cout << "Input value to search in dynamic table: ";
  while (std::cin >> value && value.getValue() != -1)
  {
    std::cout << "Search in dynamic table: " << (dynamicTable.search(value) ? "Found" : "Not found") << std::endl;
    std::cout << "Index for key " << dynamicTable.GetKey(value) << " is: " << dynamicTable.GetIndex(value) << std::endl;
  }
}

void Menu(Sequence<NIF> *table)
{
  std::cout << "1. Insertar un valor en la tabla\n";
  std::cout << "2. Buscar un valor en la tabla\n";
  std::cout << "3. Salir\n";

  std::cout << "Introduce una opción: ";

  int option;
  while (std::cin >> option)
  {
    switch (option)
      {
      case 1:
      {
        std::cout << "Introduce el NIF a insertar: ";
        NIF value;
        std::cin >> value;
        std::cout << "Insert in table: " << (table->insert(value) ? "Success" : "Failed") << std::endl;
        break;
      }
      case 2:
      {
        std::cout << "Introduce el NIF a buscar: ";
        NIF value;
        std::cin >> value;
        std::cout << "Search in table: " << (table->search(value) ? "Found" : "Not found") << std::endl;
        break;
      }
      case 3:
        std::cout << "Saliendo...\n";
        return;
      default:
        std::cout << "Opción no válida\n";
        break;
      }
    std::cout << "1. Insertar un valor en la tabla\n";
    std::cout << "2. Buscar un valor en la tabla\n";
    std::cout << "3. Salir\n";
    std::cout << "Introduce una opción: ";
  }
}

int main(int argc, char const *argv[])
{
  //Pruebas();
  auto options = parse_args(argc, argv);
  if (!options)
  {
    Usage();
    return 1;
  }

  DispersionFunction<NIF> *dispersion;
  ExplorationFunction<NIF> *exploration;
  if (options->dispersionFunction == "sum")
  {
    dispersion = new SumDispersion<NIF>(options->tableSize);
  }
  else if (options->dispersionFunction == "random")
  {
    dispersion = new PseudoRandomDispersion<NIF>(options->tableSize);
  }
  else if (options->dispersionFunction == "mod")
  {
    dispersion = new ModuloDispersion<NIF>(options->tableSize);
  }
  else
  {
    Usage();
    return 1;
  }

  if (options->explorationFunction == "linear")
  {
    exploration = new LinearExploration<NIF>;
  }
  else if (options->explorationFunction == "quadratic")
  {
    exploration = new QuadraticExploration<NIF>;
  }
  else if (options->explorationFunction == "double")
  {
    exploration = new DoubleHashExploration<NIF>(dispersion);
  }
  else if (options->explorationFunction == "rehashing")
  {
    exploration = new RehashingExploration<NIF>(dispersion);
  }
  else
  {
    Usage();
    return 1;
  }
  unsigned tableSize = options->tableSize;
  unsigned blockSize = options->blockSize;
  if (options->hashType == "open")
  {
    HashTable<NIF, dynamicSequence<NIF>> table(tableSize, *dispersion);
    Menu(&table);
    
  }
  else if (options->hashType == "close")
  {
    HashTable<NIF, staticSequence<NIF>> table(tableSize, *dispersion, *exploration, blockSize);
    Menu(&table);
  }
  else
  {
    Usage();
    return 1;
  }

  return 0;
}
