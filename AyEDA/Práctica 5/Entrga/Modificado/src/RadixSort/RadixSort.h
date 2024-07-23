/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 *
 * Algoritmos y Estructuras de Datos
 * Práctica 5: Algoritmos de ordenación
 *
 * @file RadixSort.h
 * @author Álvaro Pérez Ramos
 * @date 02 MAR 2024
 * @brief Fichero de cabecera para el método de ordenación RadixSort
 */

#ifndef __RADIXSORT_H__
#define __RADIXSORT_H__

#include "../SortMethod/SortMethod.h"
#include <algorithm>
#include <cstring>

template <class Key>
class RadixSort : public SortMethod<Key>
{
public:
  RadixSort(StaticSequence<Key> &sequence) : SortMethod<Key>(sequence) {}
  void Sort(Key *sequence, int n) const override
  {
    int max = getMax(sequence, n);
    for (int exp = 1; max / exp > 0; exp *= 10)
    {
      if (showTrace)
      {
        this->showTrace(sequence, n);
      }
      countSort(sequence, n, exp);
    }
  }

  int getMax(const Key *sequence, int n) const
  {
    int max = sequence[0];
    for (int i = 1; i < n; i++)
    {
      comparations++;
      if (sequence[i] > max)
      {
        max = sequence[i];
      }
    }
    return max;
  }

  void countSort(Key *sequence, int n, int exp) const
  {
    Key *output = new Key[n];
    int count[10] = {0};

    for (int i = 0; i < n; i++)
    {
      count[(sequence[i] / exp) % 10]++;
    }

    for (int i = 1; i < 10; i++)
    {
      count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; i--)
    {
      output[count[(sequence[i] / exp) % 10] - 1] = sequence[i];
      count[(sequence[i] / exp) % 10]--;
    }

    for (int i = 0; i < n; i++)
    {
      swaps++;
      sequence[i] = output[i];
    }

    delete[] output;
  }

  ~RadixSort() {}
};

#endif // __RADIXSORT_H__