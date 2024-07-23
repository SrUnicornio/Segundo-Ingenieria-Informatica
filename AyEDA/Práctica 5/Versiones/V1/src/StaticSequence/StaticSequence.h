/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 *
 * Algoritmos y Estructuras de Datos
 * Práctica 5: Algoritmos de ordenación
 *
 * @file StaticSequence.h
 * @author 
 * @date 02 MAR 2024
 * @brief Fichero de cabecera para la clase StaticSequence
  */

#ifndef __STATICSEQUENCE_H__
#define __STATICSEQUENCE_H__

#include "../Sequence/Sequence.h"

// Static sequence for closed addressing
template <class Key>
class StaticSequence : public Sequence<Key>
{
public:
  StaticSequence(Key *data, int n) : data_(data), n_(n) {}
  Key operator[](const Position &i) const override { return data_[i]; }
  int size() const override { return n_; }
  ~StaticSequence() {}

private:
  Key *data_;
  int n_;
};

#endif // __STATICSEQUENCE_H__