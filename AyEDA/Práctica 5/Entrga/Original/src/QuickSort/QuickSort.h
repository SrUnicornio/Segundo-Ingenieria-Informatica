/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 *
 * Algoritmos y Estructuras de Datos
 * Práctica 5: Algoritmos de ordenación
 *
 * @file QuickSort.h
 * @author Álvaro Pérez Ramos
 * @date 02 MAR 2024
 * @brief Fichero de cabecera para el método de ordenación QuickSort
 */

#ifndef __QUICKSORT_H__
#define __QUICKSORT_H__

#include "../SortMethod/SortMethod.h"
#include <algorithm>

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
      if (showTrace)
      {
        this->showTrace(sequence, fin + 1);
      }
      int p = particion(sequence, inicio, fin);
      Qsort(sequence, inicio, p - 1);
      Qsort(sequence, p + 1, fin);
    }
  }
  ~QuickSort() {}
};

#endif // __QUICKSORT_H__