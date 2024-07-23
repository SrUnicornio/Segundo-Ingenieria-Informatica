#include <array>
#include <cstdlib>
#include <iostream>
#include <vector>

/**
 * @brief Clase base para implementar las funciones de dispersión
 */
template <class Key>
class DispersionFunction
{
public:
  DispersionFunction(unsigned int size) : tableSize_(size) {}
  virtual unsigned operator()(const Key &key) const = 0;
  ~DispersionFunction() {}
  unsigned getTableSize() const { return tableSize_; }

protected:
  unsigned tableSize_;
};

/**
 * @brief Función módulo
 */
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

/**
 * @brief Función pseudoaleatoria
 */
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

/**
 * @brief Función basada en la suma
 */
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

/**
 * @brief Clase función de exploración
 */
template <class Key>
class ExplorationFunction
{
public:
  virtual unsigned operator()(const Key &, unsigned i) const = 0; // i es el intento de exploración
  virtual ~ExplorationFunction() {}
};

/**
 * @brief Clase función de doble dispersión
 */
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
  DispersionFunction<Key> f_; // Función de dispersión auxiliar f(k)
};

/**
 * @brief Clase función de exploración lineal
 */
template <class Key>
class LinearExploration : public ExplorationFunction<Key>
{
public:
  unsigned operator()(const Key &key, unsigned i) const override
  {
    return i;
  }
};

/**
 * @brief Clase función de exploración cuadrática
 */
template <class Key>
class QuadraticExploration : public ExplorationFunction<Key>
{
public:
  unsigned operator()(const Key &key, unsigned i) const override
  {
    return (i * i);
  }
};

/**
 * @brief Clase función de redispersión
 */
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
  const DispersionFunction<Key> &f_; // Función de dispersión auxiliar f(k)
};

class NIF {
  public:
    NIF() : value_(rand() % 100000000) {} // Constructor por defecto que inicializa un dni aleatorio
    NIF(long int val) : value_(val) {} // Constructor
    bool operator==(const NIF& other) const { return value_ == other.value_; }
    bool operator!=(const NIF& other) const { return !(*this == other); }
    operator long() const { return value_; }
    // Falta getOriginal -> 25418965D
  private:
    int value_; // Representación numérica del DNI
};

class NIF
{
public:
  NIF() : value_(rand() % 100000000) {} // Constructor por defecto que inicializa un dni aleatorio
  NIF(long int val) : value_(val) {}    // Constructor
  bool operator==(const NIF &other) const { return value_ == other.value_; }
  bool operator!=(const NIF &other) const { return !(*this == other); }
  operator long() const { return value_; }
  // Falta getOriginal -> 25418965D
  int getValue() const { return value_; }

private:
  int value_; // Representación numérica del DNI
};

/**
 * @brief Clase Sequence
 */
template <class Key>
class Sequence
{
public:
  virtual bool search(const Key &key) const = 0;
  virtual bool insert(const Key &key) = 0;
  virtual ~Sequence() {}
};

/**
 * @brief Clase para implementar la técnica de dispersión abierta
 */
template <class Key>
class DynamicSequence : public Sequence<Key>
{
public:
  DynamicSequence() {}
  bool search(const Key &key) const override;
  bool insert(const Key &key) override;

private:
  std::vector<Key> elements_; // Lista para almacenar los elementos en caso de colisión
};

/**
 * @brief Método para buscar un elemento en la tabla
 */
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

/**
 * @brief Método para insertar un elemento en la tabla
 */
template <class Key>
bool DynamicSequence<Key>::insert(const Key &key)
{
  if (!search(key))
  {                           // Si el elemento no está en la lista
    elements_.push_back(key); // Lo pusheamos al final
    return true;
  }
  return false;
}

/**
 * @brief Clase para implementar la técnica de dispersión cerrada
 */
template <class Key>
class StaticSequence : public Sequence<Key>
{
public:
  StaticSequence(size_t tSize, DispersionFunction<Key> &fd, ExplorationFunction<Key> &fe, size_t bSize)
      : tableSize_(tSize), fd_(fd), fe_(fe), blockSize_(bSize)
  {
    table_ = new Sequence<Key> *[tableSize_];
    for (size_t i = 0; i < tableSize_; i++)
    {
      table_[i] = nullptr;
    }
  }
  bool search(const Key &key) const override;
  bool insert(const Key &key) override;
  bool isFull() const;

private:
  size_t tableSize_;             // Tamaño de la tabla
  Sequence<Key> **table_;        // Array con tableSize posiciones, con punteros a objetos Sequence<Key>
  DispersionFunction<Key> &fd_;  // Función de dispersión
  ExplorationFunction<Key> &fe_; // Función de exploración
  size_t blockSize_;             // Tamaño del bloque
};

/**
 *@brief Método para buscar un elemento en la tabla (estática)
 */
template <class Key>
bool StaticSequence<Key>::search(const Key &key) const
{
  for (size_t i = 0; i < tableSize_; i++)
  {
    if (table_[i] == key)
    {
      return true;
    }
  }
  return false;
}

/**
 * @brief Método para insertar un elemento en la tabla (estática)
 */
template <class Key>
bool StaticSequence<Key>::insert(const Key &key)
{
  int i = 0;
  while (this->table_[i] != nullptr && i < this->blockSize_)
  {
    i++;
  }
  if (i == this->blockSize_)
    return false;
  else
    this->table_[i] = key;
  return true;
}

/**
 * @brief Método para saber si la seciencia está llena
 */
template <class Key>
bool StaticSequence<Key>::isFull() const
{
  for (size_t i = 0; i < tableSize_; i++)
  {
    if (!table_[i]->isFull())
    {
      return false; // Si al menos una posición no está llena, la secuencia no lo está
    }
  }
  return true; // Si todas las posiciones están llenas
}

template <class Key, class Container = StaticSequence<Key>>
class HashTable : public Sequence<Key>
{
public:
  HashTable(unsigned tsz, DispersionFunction<Key> &fd, ExplorationFunction<Key> &fe, unsigned bsz)
      : tableSize_(tsz), fd_(fd), fe_(fe), blockSize_(bsz)
  {
    hashTable_ = new Container *[tableSize_];
    for (size_t i = 0; i < tableSize_; i++)
    {
      hashTable_[i] = new Container{blockSize_};
    }
  }
  bool search(const Key &key) const;
  bool insert(const Key &key);

private:
  size_t tableSize_;
  DispersionFunction<Key> &fd_;
  ExplorationFunction<Key> &fe_;
  size_t blockSize_;
  Container **hashTable_;
};

/**
 * @brief Función insert estática
 */
template <class Key, class Container>
bool HashTable<Key, Container>::insert(const Key &key)
{
  int position{0};
  for (size_t i = 0; i < tableSize_; i++)
  {
    position += (fe_(key, i) + fd_(key));
    position = position % tableSize_;
    if (hashTable_[position]->insert(key) == true)
    {
      return true;
    }
  }
  return false;
}

/**
 * @brief Función search estática
 */
template <class Key, class Container>
bool HashTable<Key, Container>::search(const Key &key) const
{
  int position{0};
  for (size_t i = 0; i < tableSize_; i++)
  {
    position += (fe_(key, i) + fd_(key));
    position = position % tableSize_;
    if (hashTable_[position]->search(key) == true)
    {
      return true;
    }
  }
  return false;
}

template <class Key>
class HashTable<Key, DynamicSequence<Key>>
{
public:
  HashTable(unsigned tsz, DispersionFunction<Key> &fd) : tableSize_(tsz), fd_(fd)
  {
    hashTable_ = new DynamicSequence<Key> *[tableSize_];
  }
  bool search(const Key &key) const;
  bool insert(const Key &key);

private:
  DispersionFunction<Key> fd_;
  size_t tableSize_;
  Sequence<Key> *hashTable_;
};

/**
 * @brief Función insert dinámica
 */
template <class Key>
bool HashTable<Key, DynamicSequence<Key>>::insert(const Key &key)
{
  unsigned int position{0};
  position = fd_(key);
  position = position % tableSize_;
  if (hashTable_[position]->insert(key) == true)
  {
    return true;
  }
  return false;
}

/**
 * @brief Función search dinámica
 */
template <class Key>
bool HashTable<Key, DynamicSequence<Key>>::search(const Key &key) const
{
  unsigned int position{0};
  position = fd_(key);
  position = position % tableSize_;
  if (hashTable_[position]->search(key) == true)
  {
    return true;
  }
  return false;
}

std::istream &operator>>(std::istream &input, const NIF &nif)
{
  input >> nif.getValue();
  return input;
}

int main()
{
  NIF dni;
  ModuleDispersion<NIF> disp(100);
  QuadraticExploration<NIF> expl;
  HashTable<NIF, StaticSequence<NIF>> tabla(100, disp, expl, 3);
  NIF input;
  while (std::cin >> input)
  {
    std::cout << tabla.insert(input) << std::endl;
  }
  return 0;
}
