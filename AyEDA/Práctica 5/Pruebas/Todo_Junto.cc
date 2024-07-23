/*
Los métodos de ordenación son algoritmos que colocan los elementos de una sequence dada
según una relación de orden [2]. El resultado será un reordenamiento de la entrada que cumpla
con esa relación de orden.
● Por simplicidad, asumimos que el registro de información almacenado en la sequence
coincide con el tipo de la clave utilizada para establecer su orden Key.
Se pide implementar, al menos, los siguientes algoritmos de ordenación
Algoritmos caudratios:
      ○ Inserción
      ○ Selección
      ○ Intercambio
    Algoritmos logarítmicos:
        ○ QuickSort
        ○ HeapSort
        ○ MergeSort
      Otros algoritmos:
        ○ Por Incrementos Decrecientes (ShellSort): debe permitir seleccionar la constante de reducción alfa, siendo 0 < alfa < 1
        ○ RadixSort
        ○ TimSort

  Compilar con: g++ -std=c++14 -o PRUEBAS PRUEBAS.cc
*/
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>

static int RUN = 10;
typedef unsigned int Position;

template <class Key>
class Sequence
{
public:
  virtual Key operator[](const Position &) const = 0;
  virtual int size() const = 0;
  virtual ~Sequence() {}
};

// Static sequence for closed addressing
template <class Key>
class StaticSequence : public Sequence<Key>
{
public:
  StaticSequence(Key *data, int n) : data_(data), n_(n) {}
  Key operator[](const Position &i) const override { return data_[i]; }
  int size() const override { return n_; }
  ~StaticSequence() {}

private:
  Key *data_;
  int n_;
};

template <class Key>
class SortMethod
{
public:
  SortMethod(StaticSequence<Key> &sequence) : sequence_(sequence) {}
  virtual void Sort(Key *sequence, int n) const = 0;
  virtual ~SortMethod() {}

protected:
  StaticSequence<Key> &sequence_;
};

template <class Key>
class InsertionSort : public SortMethod<Key>
{
public:
  InsertionSort(StaticSequence<Key> &sequence) : SortMethod<Key>(sequence) {}
  void Sort(Key *sequence, int n) const override
  {
    for (int i = 1; i < n; i++)
    {
      Key x = sequence[i];
      int inicio = 0;
      int fin = i - 1;
      while (inicio <= fin)
      {
        int medio = (inicio + fin) / 2;
        if (sequence[medio] > x)
        {
          fin = medio - 1;
        }
        else
        {
          inicio = medio + 1;
        }
      }
      for (int j = i - 1; j >= inicio; j--)
      {
        sequence[j + 1] = sequence[j];
      }
      sequence[inicio] = x;
    }
  }
  ~InsertionSort() {}
};

// Cuadratic sort methods
template <class Key>
class SelectionSort : public SortMethod<Key>
{
public:
  SelectionSort(StaticSequence<Key> &sequence) : SortMethod<Key>(sequence) {}
  void Sort(Key *sequence, int n) const override
  {
    for (int i = 0; i < n - 1; i++)
    {
      int min = i;
      for (int j = i + 1; j < n; j++)
      {
        if (sequence[j] < sequence[min])
        {
          min = j;
        }
      }
      if (min != i)
      {
        std::swap(sequence[i], sequence[min]);
      }
    }
  }
  ~SelectionSort() {}
};

template <class Key>
class BubbleSort : public SortMethod<Key>
{
public:
  BubbleSort(StaticSequence<Key> &sequence) : SortMethod<Key>(sequence) {}
  void Sort(Key *sequence, int n) const override
  {
    for (int i = 0; i < n - 1; i++)
    {
      for (int j = 0; j < n - i - 1; j++)
      {
        if (sequence[j] > sequence[j + 1])
        {
          std::swap(sequence[j], sequence[j + 1]);
        }
      }
    }
  }
  ~BubbleSort() {}
};

template <class Key>
class CocktailSort : public SortMethod<Key>
{
public:
  CocktailSort(StaticSequence<Key> &sequence) : SortMethod<Key>(sequence) {}
  void Sort(Key *sequence, int n) const override
  {
    int inicio = 0;
    int fin = n - 1;
    int cam = n;
    while (inicio < fin)
    {
      for (int j = fin; j >= inicio; j--)
      {
        if (sequence[j] < sequence[j - 1])
        {
          std::swap(sequence[j - 1], sequence[j]);
          cam = j;
        }
      }
      inicio = cam + 1;
      for (int j = inicio; j <= fin; j++)
      {
        if (sequence[j] < sequence[j - 1])
        {
          std::swap(sequence[j - 1], sequence[j]);
          cam = j;
        }
      }
      fin = cam - 1;
    }
  }
  ~CocktailSort() {}
};

// Logarithmic sort methods
template <class Key>
class MergeSort : public SortMethod<Key>
{
public:
  MergeSort(StaticSequence<Key> &sequence) : SortMethod<Key>(sequence) {}
  void Sort(Key *sequence, int n) const override
  {
    mergeSort(sequence, 0, n - 1);
  }
  void merge(Key *sequence, int inicio, int medio, int fin) const
  {
    int i = inicio;
    int j = medio + 1;
    int k = inicio;
    Key aux[fin + 1];
    while (i <= medio && j <= fin)
    {
      if (sequence[i] < sequence[j])
      {
        aux[k] = sequence[i];
        i++;
      }
      else
      {
        aux[k] = sequence[j];
        j++;
      }
      k++;
    }
    if (i > medio)
    {
      while (j <= fin)
      {
        aux[k] = sequence[j];
        j++;
        k++;
      }
    }
    else
    {
      while (i <= medio)
      {
        aux[k] = sequence[i];
        i++;
        k++;
      }
    }
    for (int l = inicio; l <= fin; l++)
    {
      sequence[l] = aux[l];
    }
  }
  void mergeSort(Key *sequence, int inicio, int fin) const
  {
    if (inicio < fin)
    {
      int medio = (fin + inicio) / 2;
      mergeSort(sequence, inicio, medio);
      mergeSort(sequence, medio + 1, fin);
      merge(sequence, inicio, medio, fin);
    }
  }
  ~MergeSort() {}
};

template <class Key>
class QuickSort : public SortMethod<Key>
{
public:
  QuickSort(StaticSequence<Key> &sequence) : SortMethod<Key>(sequence) {}
  void Sort(Key *sequence, int n) const override
  {
    Qsort(sequence, 0, n - 1);
  }
  int particion(Key *sequence, int inicio, int fin) const
  {
    Key pivote = sequence[(inicio + fin) / 2];

    while (inicio <= fin)
    {
      while (sequence[inicio] < pivote)
      {
        inicio++;
      }
      while (sequence[fin] > pivote)
      {
        fin--;
      }
      if (inicio <= fin)
      {
        std::swap(sequence[inicio], sequence[fin]);
        inicio++;
        fin--;
      }
    }
    return inicio;
  }
  void Qsort(Key *sequence, int inicio, int fin) const
  {
    if (inicio < fin)
    {
      int p = particion(sequence, inicio, fin);
      Qsort(sequence, inicio, p - 1);
      Qsort(sequence, p + 1, fin);
    }
  }
  ~QuickSort() {}
};

template <class Key>
class HeapSort : public SortMethod<Key>
{
public:
  HeapSort(StaticSequence<Key> &sequence) : SortMethod<Key>(sequence) {}
  void Sort(Key *sequence, int n) const override
  {
    for (int i = n / 2 - 1; i >= 0; i--)
    {
      Baja(i, sequence, n);
    }
    for (int i = n - 1; i > 0; i--)
    {
      std::swap(sequence[0], sequence[i]);
      Baja(0, sequence, i);
    }
  }
  void Baja(int i, Key *sequence, int n) const
  {
    int h = i;
    int h1 = 2 * i + 1;
    int h2 = h1 + 1;
    if (h1 < n && sequence[h1] > sequence[h])
    {
      h = h1;
    }
    if (h2 < n && sequence[h2] > sequence[h])
    {
      h = h2;
    }

    if (h != i)
    {
      std::swap(sequence[i], sequence[h]);
      Baja(h, sequence, n);
    }
  }
  ~HeapSort() {}
};

// Other sort methods
template <class Key>
class ShellSort : public SortMethod<Key>
{
public:
  ShellSort(StaticSequence<Key> &sequence, double alpha) : SortMethod<Key>(sequence), alpha_(alpha) {}
  void Sort(Key *sequence, int n) const override
  {
    for (int gap = n / 2; gap > 0; gap *= alpha_)
    {
      for (int i = gap; i < n; i++)
      {
        Key temp = sequence[i];
        int j;
        for (j = i; j >= gap && sequence[j - gap] > temp; j -= gap)
        {
          sequence[j] = sequence[j - gap];
        }
        sequence[j] = temp;
      }
    }
  }
  ~ShellSort() {}

private:
  double alpha_;
};

template <class Key>
class TimSort : public SortMethod<Key>
{
public:
  TimSort(StaticSequence<Key> &sequence) : SortMethod<Key>(sequence) {}
  void Sort(Key *sequence, int n) const override
  {
    for (int i = 0; i < n; i += RUN)
    {
      insertionSort(sequence, i, std::min((i + RUN - 1), (n - 1)));
    }
    for (int size = RUN; size < n; size *= 2)
    {
      for (int inicio = 0; inicio < n; inicio += 2 * size)
      {
        int medio = inicio + size - 1;
        int fin = std::min(inicio + 2 * size - 1, n - 1);

        if (medio < fin)
        {
          merge(sequence, inicio, medio, fin);
        }
      }
    }
  }
  void insertionSort(Key *sequence, int inicio, int fin) const
  {
    for (int i = inicio + 1; i <= fin; i++)
    {
      Key temp = sequence[i];
      int j = i - 1;
      while (j >= inicio && sequence[j] > temp)
      {
        sequence[j + 1] = sequence[j];
        j--;
      }
      sequence[j + 1] = temp;
    }
  }
  void merge(Key *sequence, int inicio, int medio, int fin) const
  {
    int len1 = medio - inicio + 1, len2 = fin - medio;
    Key L[len1], R[len2];
    for (int i = 0; i < len1; i++)
    {
      L[i] = sequence[inicio + i];
    }
    for (int j = 0; j < len2; j++)
    {
      R[j] = sequence[medio + 1 + j];
    }
    int i = 0, j = 0, k = inicio;
    while (i < len1 && j < len2)
    {
      if (L[i] <= R[j])
      {
        sequence[k] = L[i];
        i++;
      }
      else
      {
        sequence[k] = R[j];
        j++;
      }
      k++;
    }
    while (i < len1)
    {
      sequence[k] = L[i];
      i++;
      k++;
    }
    while (j < len2)
    {
      sequence[k] = R[j];
      j++;
      k++;
    }
  }
  ~TimSort() {}
};

template <class Key>
class RadixSort : public SortMethod<Key>
{
public:
  RadixSort(StaticSequence<Key> &sequence) : SortMethod<Key>(sequence) {}
  void Sort(Key *sequence, int n) const override
  {
    RadixSortAlfanumerico(sequence, n);
  }
  void CountSortChar(char sequence[][256], int n, int exp) const
  {
    const int SIZE = 256;
    char output[n][SIZE];
    int count[SIZE] = {0};

    for (int i = 0; i < n; i++)
    {
      count[(unsigned char)sequence[i][exp]]++;
    }

    for (int i = 1; i < SIZE; i++)
    {
      count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; i--)
    {
      strcpy(output[count[(unsigned char)sequence[i][exp]] - 1], sequence[i]); // uso de strcpy para copiar la cadena
      count[(unsigned char)sequence[i][exp]]--;
    }

    for (int i = 0; i < n; i++)
    {
      strcpy(sequence[i], output[i]);
    }
  }
  int getMaxLength(char sequence[][256], int n) const
  {
    int max = strlen(sequence[0]);
    for (int i = 0; i < n; i++)
    {
      int len = strlen(sequence[i]);
      if (len > max)
      {
        max = len;
      }
    }
    return max;
  }
  void RadixSortAlfanumerico(Key sequence[], int n) const
  {
    const int SIZE = 256;
    char sequenceChar[n][SIZE];
    for (int i = 0; i < n; i++)
    {
      strcpy(sequenceChar[i], sequence[i].c_str());
    }
    int maxLength = getMaxLength(sequenceChar, n);

    for (int exp = maxLength - 1; exp >= 0; exp--)
    {
      CountSortChar(sequenceChar, n, exp);
    }

    for (int i = 0; i < n; i++)
    {
      sequence[i] = sequenceChar[i];
    }
  }
  ~RadixSort() {}
};

class NIF
{
public:
  NIF() {}
  NIF(long int nif) : nif_{nif} {}
  int getSize() { return 8; }
  operator long() { return nif_; }

private:
  long nif_;
};

template <class Key, class Func>
void imprimirSecuencia(const Key *sequence, int n, Func condition)
{
  for (int i = 0; i < n; i++)
  {
    if (condition(sequence[i][0]))
    {
      std::cout << sequence[i] << " ";
    }
  }
}

int main()
{
  std::srand(std::time(nullptr));
  std::string data[] = {"·#", "%", "@", "hola", "que", "tal", "estas", "bien", "gracias", "por", "preguntar", "90", "154", "a"};
  int n = sizeof(data) / sizeof(data[0]);

  std::cout << "Elija el algoritmo de ordenación a utilizar:\n"
            << "1. Inserción\n"
            << "2. Selección\n"
            << "3. Intercambio\n"
            << "4. MergeSort\n"
            << "5. QuickSort\n"
            << "6. HeapSort\n"
            << "7. ShellSort\n"
            << "8. RadixSort\n"
            << "9. TimSort\n"
            << "Introduzca el número de la opción: ";
  char opcion;
  std::cin >> opcion;

  switch (opcion)
  {
  case '1':
  {
    StaticSequence<std::string> sequence(data, n);
    InsertionSort<std::string> insertionSort(sequence);
    insertionSort.Sort(data, n);
    break;
  }
  case '2':
  {
    StaticSequence<std::string> sequence(data, n);
    SelectionSort<std::string> selectionSort(sequence);
    selectionSort.Sort(data, n);
    break;
  }
  case '3':
  {
    StaticSequence<std::string> sequence(data, n);
    std::cout << "Elija el algoritmo de intercambio a utilizar:\n"
              << "1. Burbuja\n"
              << "2. Sacudida\n"
              << "Introduzca el número de la opción: ";
    std::cin >> opcion;
    switch (opcion)
    {
    case '1':
    {
      BubbleSort<std::string> bubbleSort(sequence);
      bubbleSort.Sort(data, n);
      break;
    }
    case '2':
    {
      CocktailSort<std::string> cocktailSort(sequence);
      cocktailSort.Sort(data, n);
      break;
    }
    default:
      std::cout << "Opción no válida" << std::endl;
      break;
    }
    break;
  }
  case '4':
  {
    StaticSequence<std::string> sequence(data, n);
    MergeSort<std::string> mergeSort(sequence);
    mergeSort.Sort(data, n);
    break;
  }
  case '5':
  {
    StaticSequence<std::string> sequence(data, n);
    QuickSort<std::string> quickSort(sequence);
    quickSort.Sort(data, n);
    break;
  }
  case '6':
  {
    StaticSequence<std::string> sequence(data, n);
    HeapSort<std::string> heapSort(sequence);
    heapSort.Sort(data, n);
    break;
  }
  case '7':
  {
    double alpha;
    std::cout << "Introduzca el valor de alpha: ";
    std::cin >> alpha;
    StaticSequence<std::string> sequence(data, n);
    ShellSort<std::string> shellSort(sequence, alpha);
    shellSort.Sort(data, n);
    break;
  }
  case '8':
  {
    StaticSequence<std::string> sequence(data, n);
    RadixSort<std::string> radixSort(sequence);
    radixSort.Sort(data, n);
    break;
  }
  case '9':
  {
    StaticSequence<std::string> sequence(data, n);
    TimSort<std::string> timSort(sequence);
    timSort.Sort(data, n);
    break;
  }
  default:
  {
    std::cout << "Opción no válida" << std::endl;
    break;
  }
  }

  std::cout << "¿Cómo desea que sea el orden de la sequence?\n"
            << "1. letras, números y otros caracteres\n"
            << "2. números, letras y otros caracteres\n"
            << "3. letras, otros caracteres y números\n"
            << "4. números, otros caracteres y letras\n"
            << "5. otros caracteres, letras y números\n"
            << "6. otros caracteres, números y letras\n";
  opcion;
  std::cin >> opcion;

  switch (opcion)
  {
  case '1':
    imprimirSecuencia(data, n, [](char c)
                      { return std::isalpha(c); });
    imprimirSecuencia(data, n, [](char c)
                      { return std::isdigit(c); });
    imprimirSecuencia(data, n, [](char c)
                      { return !std::isalpha(c) && !std::isdigit(c); });
    break;
  case '2':
    imprimirSecuencia(data, n, [](char c)
                      { return std::isalpha(c); });
    imprimirSecuencia(data, n, [](char c)
                      { return !std::isalpha(c) && !std::isdigit(c); });
    imprimirSecuencia(data, n, [](char c)
                      { return std::isdigit(c); });
    break;
  case '3':
    imprimirSecuencia(data, n, [](char c)
                      { return std::isdigit(c); });
    imprimirSecuencia(data, n, [](char c)
                      { return std::isalpha(c); });
    imprimirSecuencia(data, n, [](char c)
                      { return !std::isalpha(c) && !std::isdigit(c); });
    break;
  case '4':
    imprimirSecuencia(data, n, [](char c)
                      { return std::isdigit(c); });
    imprimirSecuencia(data, n, [](char c)
                      { return !std::isdigit(c) && std::isalpha(c); });
    imprimirSecuencia(data, n, [](char c)
                      { return !std::isalpha(c); });
    break;
  case '5':
    imprimirSecuencia(data, n, [](char c)
                      { return !std::isalpha(c) && !std::isdigit(c); });
    imprimirSecuencia(data, n, [](char c)
                      { return std::isalpha(c); });
    imprimirSecuencia(data, n, [](char c)
                      { return std::isdigit(c); });
    break;
  case '6':
    imprimirSecuencia(data, n, [](char c)
                      { return !std::isdigit(c) && !std::isalpha(c); });
    imprimirSecuencia(data, n, [](char c)
                      { return std::isdigit(c); });
    imprimirSecuencia(data, n, [](char c)
                      { return std::isalpha(c); });
    break;
  default:
    std::cout << "Opción no válida" << std::endl;
    break;
  }
  std::cout << std::endl;

  return 0;
}
