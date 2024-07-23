/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asigantura: Algoritmos y Estructuras de Datos Avanzadas
 * 
 * @date: 03 FEB 2024
 * @author:
 * @file Lattice.hpp
 * @brief Declaración de la clase Lattice
 */

#ifndef __LATTICE_HPP__
#define __LATTICE_HPP__

#include "Cell.hpp"
#include <fstream>
#include <iostream>
#include <optional>
#include <string>

typedef int State;    // 0 = muerta, 1 = viva
typedef int Position; // LCR (left, center, right)

class Cell;
class Lattice
{
public:
  Lattice(Cell[], int, const std::string &, const std::optional<State>);
  ~Lattice() { delete cell_; }

  int GetSize() const { return size_; }
  std::string GetBorder() const { return border_; }
  std::optional<State> GetV() const { return v_; }

  void Load(const std::string &);
  const Cell &GetCell(const Position &position) const { return cell_[position]; }
  void NextGeneration();

  friend std::ostream &operator<<(std::ostream &, const Lattice &);

private:
  int size_;
  std::string border_;
  std::optional<State> v_;
  Cell *cell_;
};

#endif // __LATTICE_HPP__
