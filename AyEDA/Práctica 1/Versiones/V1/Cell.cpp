/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asigantura: Algoritmos y Estructuras de Datos Avanzadas
 * 
 * @date: 03 FEB 2024
 * @author:
 * @file cell.cpp
 * @brief Implementación de la clase Cell
*/

#include "Cell.hpp"
#include "Lattice.hpp"

/**
 * @brief Set the state of the cell
 * @param state
 * @return void
 */

State Cell::SetState(State state)
{
  state_ = state;
  return state_;
}

/**
 * @brief Calculates the state of the cell in the next generation
 * @param lattice
 * @return int
 */

int Cell::NextState(const Lattice &lattice, const Position &position)
{
  int size = lattice.GetSize();
  std::string border = lattice.GetBorder();
  std::optional<State> v = lattice.GetV();
  State nextstate = DEAD;
  if (border == "open" || border == "o")
  {
    if (position == 0)
    {
      left_ = v.value();
      right_ = lattice.GetCell(position + 1).GetState();
    }
    else if (position == size - 1)
    {
      left_ = lattice.GetCell(position - 1).GetState();
      right_ = v.value();
    }
    else
    {
      left_ = lattice.GetCell(position - 1).GetState();
      right_ = lattice.GetCell(position + 1).GetState();
    }
  }
  else if (border == "periodic" || border == "p")
  {
    if (position == 0)
    {
      left_ = lattice.GetCell(size - 1).GetState();
      right_ = lattice.GetCell(position + 1).GetState();
    }
    else if (position == size - 1)
    {
      left_ = lattice.GetCell(position - 1).GetState();
      right_ = lattice.GetCell(0).GetState();
    }
    else
    {
      left_ = lattice.GetCell(position - 1).GetState();
      right_ = lattice.GetCell(position + 1).GetState();
    }
  }
  else if (border == "reflective" || border == "r")
  {
    if (position == 0)
    {
      left_ = lattice.GetCell(position).GetState();
      right_ = lattice.GetCell(position + 1).GetState();
    }
    else if (position == size - 1)
    {
      left_ = lattice.GetCell(position - 1).GetState();
      right_ = lattice.GetCell(position).GetState();
    }
    else
    {
      left_ = lattice.GetCell(position - 1).GetState();
      right_ = lattice.GetCell(position + 1).GetState();
    }
  }
  nextstate = ((state_ + right_) + (state_ * right_) + (state_ * left_ * right_)) % 2;
  nextstate_ = nextstate;
  return nextstate;
}

/**
 * @brief Overload of the insertion operator in flow
 * The cell is responsible for its display on the screen, which it does by overloading the insertion operator in flow. To facilitate the display, the character 'X' will be used to represent the state value "1", and the space character ' ' for the state value "0".
 * @param os
 * @param cell
 * @return State of the cell
 */

std::ostream &operator<<(std::ostream &os, const Cell &cell)
{
  os << (cell.GetState() == ALIVE ? 'L' : 'D'); // D = dead, L = alive
  return os;
}