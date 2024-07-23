/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 *
 * Algoritmos y Estructuras de Datos
 * Práctica 5: Algoritmos de ordenación
 *
 * @file ShellSort.h
 * @author Álvaro Pérez Ramos
 * @date 02 MAR 2024
 * @brief Fichero de cabecera para el método de ordenación ShellSort
 */

#ifndef __SHELLSORT_H__
#define __SHELLSORT_H__

#include "../SortMethod/SortMethod.h"
#include <algorithm>
#include <stdexcept>

template <class Key>
class ShellSort : public SortMethod<Key>
{
public:
  ShellSort(StaticSequence<Key> &sequence, double alpha) : SortMethod<Key>(sequence), alpha_(alpha) {}
  void Sort(Key *sequence, int n) const override
  {
    for (int gap = n / 2; gap > 0; gap *= alpha_)
    {
      if (showTrace)
      {
        this->showTrace(sequence, n);
      }
      for (int i = gap; i < n; i++)
      {
        Key temp = sequence[i];
        int j;
        for (j = i; j >= gap && sequence[j - gap] > temp; j -= gap)
        {
          comparations++; // Increment comparations
          sequence[j] = sequence[j - gap];
          swaps++; // Increment swaps after each actual swap
        }
        comparations++; // Increment comparations for the last comparison in the inner loop
        sequence[j] = temp;
        swaps++; // Increment swaps after each actual swap
      }
    }
  }

  ~ShellSort() {}

private:
  double alpha_;
};

#endif // __SHELLSORT_H__
