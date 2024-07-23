/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 *
 * Algoritmos y Estructuras de Datos
 * Práctica 5: Algoritmos de ordenación
 *
 * @file SelectionSort.h
 * @author 
 * @date 02 MAR 2024
 * @brief Fichero de cabecera para el método de ordenación SelectionSort
 */

#ifndef __SELECTIONSORT_H__
#define __SELECTIONSORT_H__

#include "../SortMethod/SortMethod.h"
#include <algorithm>

template <class Key>
class SelectionSort : public SortMethod<Key>
{
public:
  SelectionSort(StaticSequence<Key> &sequence) : SortMethod<Key>(sequence) {}
  void Sort(Key *sequence, int n) const override
  {
    for (int i = 0; i < n - 1; i++)
    {
      if (showTrace)
      {
        this->showTrace(sequence, n);
      }
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

#endif // __SELECTIONSORT_H__
