#ifndef __FactoryCell_HPP__
#define __FactoryCell_HPP__

#include "Cell.hpp"

class FactoryCell
{
public:
  virtual Cell *CreateCell(const Position &position, const State &state) const = 0;
};

// Derived class FactoryCellACE110 and FactoryCellACE30
class FactoryCellACE110 : public FactoryCell
{
public:
  // Virtual method to create a cell
  Cell *CreateCell(const Position &position, const State &state) const { return new CellACE110(position, state); }
};

class FactoryCellACE30 : public FactoryCell
{
public:
  // Virtual method to create a cell
  Cell *CreateCell(const Position &position, const State &state) const { return new CellACE30(position, state); }
};

class FactoryCellLife23_3 : public FactoryCell
{
public:
  Cell *CreateCell(const Position &position, const State &state) const { return new CellLive23_3(position, state); }
};

class FactoryCellLife51_346 : public FactoryCell
{
public:
  Cell *CreateCell(const Position &position, const State &state) const { return new CellLife51_346(position, state); }
};

#endif // __FactoryCell_HPP__