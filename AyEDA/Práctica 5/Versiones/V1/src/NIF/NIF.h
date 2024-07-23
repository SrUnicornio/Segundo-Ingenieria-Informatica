/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * 
 * Algoritmos y Estructuras de Datos
 * Práctica 5: Algoritmos de ordenación
 * 
 * @file NIF.h
 * @author 
 * @date 02 MAR 2024
 * @brief Fichero de cabecera para la clase NIF
*/

#ifndef __NIF_H__
#define __NIF_H__

#include <iostream>
#include <cstdlib>
#include <ctime>

class NIF
{
public:
  NIF() : nif_{rand() % 100000000} {}
  NIF(long int nif) : nif_{nif} {}
  int getSize() { return 8; }
  long int getNif() { return nif_; }
  operator long() const { return nif_; }

  friend std::istream &operator>>(std::istream &is, NIF &nif)
  {
    std::cout << "\nEnter NIF value: ";
    is >> nif.nif_;
    return is;
  }
  friend std::ostream &operator<<(std::ostream &os, const NIF &nif)
  {
    os << nif.nif_;
    return os;
  }

private:
  long nif_;
};

#endif // __NIF_H__