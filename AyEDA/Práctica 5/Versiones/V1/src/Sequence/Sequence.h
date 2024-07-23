/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 *
 * Algoritmos y Estructuras de Datos
 * Práctica 5: Algoritmos de ordenación
 *
 * @file Sequence.h
 * @author 
 * @date 02 MAR 2024
 * @brief Fichero de cabecera para la clase abstracta Sequence
 */

#ifndef __SEQUENCE_H__
#define __SEQUENCE_H__

typedef unsigned int Position;

template <class Key>
class Sequence
{
public:
  virtual Key operator[](const Position &) const = 0;
  virtual int size() const = 0;
  virtual ~Sequence() {}
};

#endif // __SEQUENCE_H__