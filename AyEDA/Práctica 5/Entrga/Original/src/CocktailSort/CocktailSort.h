/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * 
 * Algoritmos y Estructuras de Datos
 * Práctica 5: Algoritmos de ordenación
 * 
 * @file CocktailSort.h
 * @author Álvaro Pérez Ramos
 * @date 02 MAR 2024
 * @brief Fichero de cabecera para el método de ordenación CocktailSort
 */


#ifndef __COCKTAILSORT_H__
#define __COCKTAILSORT_H__

#include "../SortMethod/SortMethod.h"
#include <algorithm>

template <class Key>
class CocktailSort : public SortMethod<Key>
{
public:
  CocktailSort(StaticSequence<Key> &sequence) : SortMethod<Key>(sequence) {}
  void Sort(Key *sequence, int n) const override
  {
    int inicio = 1;
    int fin = n - 1;
    int cam = n;
    while (inicio < fin)
    {
      if (showTrace)
      {
        this->showTrace(sequence, n);
      }
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

#endif // __COCKTAILSORT_H__