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
int Cell::CountAlive(const Board &Board, const Position &position)
{
  int count = 0;
  for (int i = position.first - 1; i <= position.first + 1; i++)
  {
    for (int j = position.second - 1; j <= position.second + 1; j++)
    {
      if (i != position.first || j != position.second)
      {
        if (Board.GetCellState(std::make_pair(i, j)) == kAlive)
        {
          count++;
        }
      }
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
