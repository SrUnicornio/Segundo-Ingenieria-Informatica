/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 *
 * Algoritmos y Estructuras de Datos
 * Práctica 5: Algoritmos de ordenación
 *
 * @file SortMethod.h
 * @author 
 * @date 02 MAR 2024
 * @brief Fichero de cabecera para la clase abstracta SortMethod
 */

#ifndef __SORTMETHOD_H__
#define __SORTMETHOD_H__

#include "../StaticSequence/StaticSequence.h"
#include <iostream>

static bool showTrace = false;

template <class Key>
class SortMethod
{
public:
  SortMethod(StaticSequence<Key> &sequence) : sequence_(sequence) {}
  virtual void Sort(Key *sequence, int n) const = 0;
  virtual ~SortMethod() {}

  void showTrace(const Key *sequence, int n) const
  {
    for (int i = 0; i < n; i++)
    {
      std::cout << sequence[i] << " ";
    }
    std::cout << std::endl;
  }

protected:
  StaticSequence<Key> &sequence_;
};

#endif // __SORTMETHOD_H__