/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 *
 * Algoritmos y Estructuras de Datos
 * Práctica 5: Algoritmos de ordenación
 *
 * @file MergeSort.h
 * @author Álvaro Pérez Ramos
 * @date 02 MAR 2024
 * @brief Fichero de cabecera para el método de ordenación MergeSort
 */

#ifndef __MERGESORT_H__
#define __MERGESORT_H__

#include "../SortMethod/SortMethod.h"
#include <algorithm>

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
    Key *aux = new Key[fin + 1];
    while (i <= medio && j <= fin)
    {
      comparations++; // Increment comparisons
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

    delete[] aux;
  }

  void mergeSort(Key *sequence, int inicio, int fin) const
  {
    if (inicio < fin)
    {
      if (showTrace)
      {
        this->showTrace(sequence, fin + 1);
      }
      int medio = (fin + inicio) / 2;
      mergeSort(sequence, inicio, medio);
      mergeSort(sequence, medio + 1, fin);
      merge(sequence, inicio, medio, fin);
    }
  }
  ~MergeSort() {}
};

#endif // __MERGESORT_H__