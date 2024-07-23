/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asigantura: Algoritmos y Estructuras de Datos Avanzadas
 * 
 * @date: 03 FEB 2024
 * @author: Álvaro Pérez Ramos
 * @file Cell.hpp
 * @brief Declaración de la clase Cell
 */

#ifndef __CELL_HPP__
#define __CELL_HPP__

#include <iostream>
#include <optional>
#include <string>

typedef int State;    // 0 = muerta, 1 = viva
const State ALIVE = 1;
const State DEAD = 0;
typedef int Position; // LCR (left, center, right)


class Lattice;
class Cell
{
public:
  Cell() {}
  Cell(const Position &position, const State &state = 0) : position_(position), state_(state) {}

  State GetState() const { return state_; }
  State SetState(State);

  int NextState(const Lattice &lattice, const Position &position);
  void UpdateState() { state_ = SetState(nextstate_); }

  friend std::ostream &operator<<(std::ostream &, const Cell&);

private:
  Position position_;
  State state_;
  State left_;
  State right_;
  State nextstate_;
};

#endif // __CELL_HPP__