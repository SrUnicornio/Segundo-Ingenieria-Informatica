/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 *
 * Algoritmos y Estructuras de Datos
 * Práctica 5: Algoritmos de ordenación
 *
 * @file TimSort.h
 * @author 
 * @date 02 MAR 2024
 * @brief Fichero de cabecera para el método de ordenación TimSort
 */

#ifndef __TIMSORT_H__
#define __TIMSORT_H__

#include "../SortMethod/SortMethod.h"
#include <algorithm>

static int RUN = 32;

template <class Key>
class TimSort : public SortMethod<Key>
{
public:
  TimSort(StaticSequence<Key> &sequence) : SortMethod<Key>(sequence) {}
  void Sort(Key *sequence, int n) const override
  {
    for (int i = 0; i < n; i += RUN)
    {
      if (showTrace)
      {
        this->showTrace(sequence, n);
      }
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
          if (showTrace)
          {
            this->showTrace(sequence, n);
          }
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
    Key *L = new Key[len1], *R = new Key[len2];
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

    delete[] L;
    delete[] R;
  }
  ~TimSort() {}
};

#endif // __TIMSORT_H__