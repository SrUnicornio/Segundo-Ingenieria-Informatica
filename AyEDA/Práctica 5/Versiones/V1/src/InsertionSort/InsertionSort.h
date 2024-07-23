/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 *
 * Algoritmos y Estructuras de Datos
 * Práctica 5: Algoritmos de ordenación
 *
 * @file InsertionSort.h
 * @author
 * @date 02 MAR 2024
 * @brief Fichero de cabecera para el método de ordenación InsertionSort
 */

#ifndef __INSERTIONSORT_H__
#define __INSERTIONSORT_H__

#include "../SortMethod/SortMethod.h"

template <class Key>
class InsertionSort : public SortMethod<Key>
{
public:
  InsertionSort(StaticSequence<Key> &sequence) : SortMethod<Key>(sequence) {}
  void Sort(Key *sequence, int n) const override
  {
    for (int i = 1; i < n; i++)
    {
      if (showTrace)
      {
        this->showTrace(sequence, n);
      }
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

#endif // __INSERTIONSORT_H__