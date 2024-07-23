/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asigantura: Algoritmos y Estructuras de Datos Avanzadas
 *
 * @date: 12 FEB 2024
 * @author: Álvaro Pérez Ramos
 * @file Cell.cpp
 * @brief Definiton of methods of the class Cell
 * 
 * */

#include "../include/Cell.hpp"
#include "../include/Board.hpp"

/**
 * @brief Calculates the next state of the cell
 * @param Board
 * @param position
 * @return Next state of the cell
 * @note The rules of the game are as follows:
 * 23/3: A living cell with two or three living neighbors remains alive.
 *       A dead cell with three living neighbors becomes alive.
 */
int Cell::NextState(const Board &board, const Position &position)
{
  int count_cells_alive = CountAlive(board, position);
  if (state_ == kAlive)
  {
    if (count_cells_alive < 2 || count_cells_alive > 3)
    {
      nextstate_ = kDead;
    }
    else
    {
      nextstate_ = kAlive;
    }
  }
  else
  {
    if (count_cells_alive == 3)
    {
      nextstate_ = kAlive;
    }
    else
    {
      nextstate_ = kDead;
    }
  }
  return nextstate_;
}

/**
 * @brief Counts the number of living cells
 * @param Board
 * @param position
 * @return Number of living cells
 */
int Cell::CountAlive(const Board &board, const Position &position)
{
  int count = 0;
  std::vector<Position> adjacent_positions = {
      {position.first - 1, position.second - 1}, // 1
      {position.first - 1, position.second},     // 2
      {position.first - 1, position.second + 1}, // 3
      {position.first, position.second + 1},     // 4
      {position.first + 1, position.second + 1}, // 5
      {position.first + 1, position.second},     // 6
      {position.first + 1, position.second - 1}, // 7
      {position.first, position.second - 1}      // 8
  };
  // Contar las celdas vivas
  for (auto &adjacent_position : adjacent_positions)
  {
    if (board.GetCellState(adjacent_position) == kAlive)
    {
      count++;
    }
  }
  return count;
}

/**
 * @brief Overload of the insertion operator in flow
 */
std::ostream &operator<<(std::ostream &os, const Cell &cell)
{
  os << (cell.GetState() == kAlive ? 'X' : ' ');
  return os;
}
