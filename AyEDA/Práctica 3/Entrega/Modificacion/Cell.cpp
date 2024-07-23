#include "Cell.hpp"
#include "Lattice.hpp"
#include <vector>
#include <set>

std::ostream &operator<<(std::ostream &os, const Cell &cell)
{
  cell.DisplayCell(os);
  return os;
}

void CellACE110::NextState(const Lattice &lattice, const Position &position)
{
  State left = kDead;
  State right = kDead;
  std::string border = lattice.GetBorder();
  std::optional<State> v = lattice.GetV();

  left = lattice.GetCellState(PositionDim<1>(position[0] - 1));
  right = lattice.GetCellState(PositionDim<1>(position[0] + 1));

  nextstate_ = ((state_ + right) + (state_ * right) + (state_ * left * right)) % 2;
}

void CellACE30::NextState(const Lattice &lattice, const Position &position)
{
  State left = kDead;
  State right = kDead;
  std::string border = lattice.GetBorder();
  std::optional<State> v = lattice.GetV();

  left = lattice.GetCellState(PositionDim<1>(position[0] - 1));
  right = lattice.GetCellState(PositionDim<1>(position[0] + 1));

  nextstate_ = ((left + state_ + right) + (state_ * right)) % 2;
}

void CellACE::DisplayCell(std::ostream &os) const
{
  os << (state_ == kAlive ? "X" : " ");
}

void CellLife::DisplayCell(std::ostream &os) const
{
  os << (state_ == kAlive ? "X" : " ");
}

void CellLive23_3::NextState(const Lattice &lattice, const Position &position)
{
  int alive = CountAlive(lattice, position);
  if (state_ == kAlive)
  {
    nextstate_ = (alive == 2 || alive == 3) ? kAlive : kDead;
  }
  else
  {
    nextstate_ = (alive == 3) ? kAlive : kDead;
  }
}

void CellLife51_346::NextState(const Lattice &lattice, const Position &position)
{
  int alive = CountAlive(lattice, position);
  if (state_ == kAlive)
  {
    nextstate_ = (alive == 5 || alive == 1) ? kAlive : kDead;
  }
  else
  {
    nextstate_ = (alive == 3 || alive == 4 || alive == 6) ? kAlive : kDead;
  }
}

int CellLife::CountAlive(const Lattice &lattice, const Position &position) const
{
  int alive = 0;
  std::vector<PositionDim<2>> positions = {
      PositionDim<2>(position[0] - 1, position[1] - 1),
      PositionDim<2>(position[0] - 1, position[1]),
      PositionDim<2>(position[0] - 1, position[1] + 1),
      PositionDim<2>(position[0], position[1] - 1),
      PositionDim<2>(position[0], position[1] + 1),
      PositionDim<2>(position[0] + 1, position[1] - 1),
      PositionDim<2>(position[0] + 1, position[1]),
      PositionDim<2>(position[0] + 1, position[1] + 1)};
  for (const auto &p : positions)
  {
    if (lattice.GetCellState(p) == kAlive)
    {
      alive++;
    }
  }
  return alive;
}
