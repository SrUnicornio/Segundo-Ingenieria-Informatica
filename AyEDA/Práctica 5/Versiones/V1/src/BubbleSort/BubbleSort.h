/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * 
 * Algoritmos y Estructuras de Datos
 * Práctica 5: Algoritmos de ordenación
 * 
 * @file BubbleSort.h
 * @author 
 * @date 02 MAR 2024
 * @brief Fichero de cabecera para el método de ordenación de la burbuja
*/

#ifndef __BUBBLESORT_H__
#define __BUBBLESORT_H__

#include "../SortMethod/SortMethod.h"
#include <algorithm>

template <class Key>
class BubbleSort : public SortMethod<Key>
{
public:
  BubbleSort(StaticSequence<Key> &sequence) : SortMethod<Key>(sequence) {}
  void Sort(Key *sequence, int n) const override
  {
    for (int i = 0; i < n - 1; i++)
    {
      if (showTrace)
      {
        this->showTrace(sequence, n);
      }
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

#endif // __BUBBLESORT_H__