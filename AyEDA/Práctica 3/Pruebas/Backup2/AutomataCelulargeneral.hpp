#ifndef __AUTOMATACELULARGENERAL_HPP__
#define __AUTOMATACELULARGENERAL_HPP__

#include <algorithm>
#include <cassert>
#include <cstdarg>
#include <fstream>
#include <iostream>
#include <optional>
#include <set>
#include <stdlib.h>
#include <string>
#include <thread>
#include <time.h>
#include <utility>
#include <vector>

static bool already_expanded_back = false;
static bool already_expanded_front = false;
static bool already_expanded_top = false;
static bool already_expanded_bottom = false;

// Definition of the state of a cell
typedef int State;
const State kAlive = 1;
const State kDead = 0;

// Definition of abstract class Position
typedef int Coor_t;
class Position
{
public:
  // Operator of access to the coordinates
  // virtual Coor_t operator[](unsigned int) const = 0; me da error
  virtual Coor_t operator[](unsigned int) const { return 0; }
};

// Derived class PositionDim
template <int Dim = 2, class Coordinate_t = int>
class PositionDim : public Position
{
private:
  Coor_t Coordinates[Dim];

public:
  // Constructor con lista variable de parámetros
  PositionDim(int sz, ...)
  {
    va_list vl;
    va_start(vl, sz);
    for (int d = 0; d < Dim; d++)
    {
      Coordinates[d] = va_arg(vl, Coor_t);
    }
    va_end(vl);
  }

  Coor_t operator[](unsigned int) const override;
  bool operator<(const PositionDim<Dim, Coordinate_t> &other) const
  {
    for (int i = 0; i < Dim; ++i)
    {
      if (Coordinates[i] < other.Coordinates[i])
        return true;
      if (Coordinates[i] > other.Coordinates[i])
        return false;
    }
    return false;
  }
};

class ac_exception : public std::exception
{
public:
  const char *what() const throw()
  {
    return "Error: Coordinate out of range";
  }
};

// Definition of the abstract class Cell
// Pre-declaration of the class Lattice
class Lattice;
class Cell
{
public:
  // Constructor and destructor
  Cell() = default;
  Cell(const Position &position, const State &state = kDead) : position_(position), state_(state) {}
  virtual ~Cell() {}

  // Getters and setters
  State GetState() const { return state_; }
  State SetState(State state) { return state_ = state; }

  // Virtual method to evolve the state of the cell
  virtual void NextState(const Lattice &, const Position &) = 0;
  virtual void UpdateState() = 0;
  virtual void DisplayCell(std::ostream &) const = 0;
  friend std::ostream &operator<<(std::ostream &, const Cell &);

protected:
  const Position &position_;
  State state_;
  State nextstate_;
};

class CellLife : public Cell
{
public:
  CellLife(const Position &position, const State &state = kDead) : Cell(position, state) {}
  ~CellLife() {}

  virtual void NextState(const Lattice &, const Position &) = 0;
  int CountAlive(const Lattice &, const Position &) const;
  void UpdateState() { SetState(nextstate_); }
  void DisplayCell(std::ostream &) const;
};

// Derived class CellACE
class CellACE : public Cell
{
public:
  // Constructor and destructor
  CellACE(const Position &position, const State &state) : Cell(position, state) {}
  ~CellACE() {}

  // Virtual methods to evolve the state of the cell
  virtual void NextState(const Lattice &, const Position &) = 0;
  void UpdateState() { SetState(nextstate_); }
  void DisplayCell(std::ostream &) const;
};

// Derived class CellACE110 and CellACE30
class CellACE110 : public CellACE
{
public:
  // Constructor
  CellACE110(const Position &position, const State &state) : CellACE(position, state) {}
  ~CellACE110() {}

  // Virtual methods to evolve the state of the cell
  void NextState(const Lattice &, const Position &) override;
};

class CellACE30 : public CellACE
{
public:
  // Constructor
  CellACE30(const Position &position, const State &state) : CellACE(position, state) {}
  ~CellACE30() {}

  // Virtual methods to evolve the state of the cell
  void NextState(const Lattice &, const Position &) override;
};

class CellLive23_3 : public CellLife
{
public:
  CellLive23_3(const Position &position, const State &state = kDead) : CellLife(position, state) {}
  ~CellLive23_3() {}

  void NextState(const Lattice &, const Position &);
};

class CellLife51_346 : public CellLife
{
public:
  CellLife51_346(const Position &position, const State &state = kDead) : CellLife(position, state) {}
  ~CellLife51_346() {}

  void NextState(const Lattice &, const Position &);
};

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

class Lattice
{
public:
  Lattice() = default;
  Lattice(const Position &position, const FactoryCell &factory, const std::string &border, const std::optional<State> &v = kDead) : position_(position), factory_(factory), border_(border), v_(v) {}
  Lattice(const char *filename, const FactoryCell &factory, const std::string &border, const std::optional<State> &v = kDead) : filename_(filename), position_(PositionDim<2>(0, 0)), factory_(factory), border_(border), v_(v) {}
  virtual ~Lattice() {}

  const Position &GetPosition() const { return position_; }
  const FactoryCell &GetFactory() const { return factory_; }
  const std::string &GetBorder() const { return border_; }
  const std::optional<State> &GetV() const { return v_; }

  virtual const State GetCellState(const Position &) const = 0;
  virtual const Cell &operator[](const Position &) const = 0;
  virtual void NextGeneration() = 0;
  virtual std::size_t Population() const = 0;
  virtual void DisplayLattice(std::ostream &) const = 0;
  friend std::ostream &operator<<(std::ostream &, const Lattice &);

protected:
  const char *filename_;
  const Position position_;
  const FactoryCell &factory_;
  std::string border_;
  std::optional<State> v_;
  Cell ***cell2D_;
  Cell **cell1D_;
};

// Derived class Lattice1D
class Lattice1D : public Lattice
{
public:
  // Constructor and destructor
  Lattice1D(const Position &position, const FactoryCell &factory, const std::string &border, const std::optional<State> &v);
  Lattice1D(const char *filename, const FactoryCell &factory, const std::string &border, const std::optional<State> &v);
  ~Lattice1D();

  // Virtual methods to access the cells of the lattice
  const Cell &operator[](const Position &position) const { return *cell1D_[position[0]]; }
  void NextGeneration();
  std::size_t Population() const;
  void DisplayLattice(std::ostream &) const;
  const State GetCellState(const Position &position) const;
  // void RunGame();

protected:
  int size_ = position_[0];

private:
  const State OpenBoder(const Position &position) const;
  const State PeriodicBoder(const Position &position) const;
};

// Derived class Lattice1D_open and Lattice1D_periodic
class Lattice1D_open : public Lattice1D
{
public:
  // Constructor
  Lattice1D_open(const Position &position, const FactoryCell &factory, const std::string &border, const std::optional<State> &v) : Lattice1D(position, factory, border, v) {}
  Lattice1D_open(const char *filename, const FactoryCell &factory, const std::string &border, const std::optional<State> &v) : Lattice1D(filename, factory, border, v) {}
};

class Lattice1D_periodic : public Lattice1D
{
public:
  // Constructor
  Lattice1D_periodic(const Position &position, const FactoryCell &factory, const std::string &border, const std::optional<State> &v) : Lattice1D(position, factory, border, v) {}
  Lattice1D_periodic(const char *filename, const FactoryCell &factory, const std::string &border, const std::optional<State> &v) : Lattice1D(filename, factory, border, v) {}
};

class Lattice2D : public Lattice
{
public:
  Lattice2D(const Position &position, const FactoryCell &factory, const std::string &border, const std::optional<State> &v = kDead);
  Lattice2D(const char *filename, const FactoryCell &factory, const std::string &border, const std::optional<State> &v = kDead);
  ~Lattice2D() { CleanLattice2D(); }

  const State GetCellState(const Position &) const override;
  void SetRows(int rows) { rows_ = rows; }
  void SetCols(int cols) { cols_ = cols; }

  const Cell &operator[](const Position &) const override { return *cell2D_[rows_][cols_]; }
  virtual void NextGeneration() = 0;
  std::size_t Population() const override;
  void DisplayLattice(std::ostream &) const override;
  void CleanLattice2D();
  void ComputeNextState();
  void UpdateCellStates();

protected:
  int rows_ = position_[0];
  int cols_ = position_[1];

private:
  const State NoBoreder(const Position &) const;
  const State Reflective(const Position &) const;
  int ReflectCoordinate(int, int) const;
};

class Lattice2D_noborder : public Lattice2D
{
public:
  Lattice2D_noborder(const Position &position, const FactoryCell &factory, const std::string &border, const std::optional<State> &v = kDead) : Lattice2D(position, factory, border, v) {}
  Lattice2D_noborder(const char *filename, const FactoryCell &factory, const std::string &border, const std::optional<State> &v = kDead) : Lattice2D(filename, factory, border, v) {}
  ~Lattice2D_noborder() {}

  void NextGeneration() override;

private:
  void Push_back();
  void Push_front();
  void Push_top();
  void Push_bottom();
  void Push_at(const Position &position);
  void CopyLattice2D(Cell ***, Cell ***&, int, int);
  void IdentifyNextAliveCells(std::set<PositionDim<2>>& next_alive_cell);
  void PushAtBorders(const std::set<PositionDim<2>>& next_alive_cell);
  void ResetExpansionFlags();
  void PushCellAtBorder(int row, int col);
};

class Lattice2D_reflective : public Lattice2D
{
public:
  Lattice2D_reflective(const Position &position, const FactoryCell &factory, const std::string &border, const std::optional<State> &v = kDead) : Lattice2D(position, factory, border, v) {}
  Lattice2D_reflective(const char *filename, const FactoryCell &factory, const std::string &border, const std::optional<State> &v = kDead) : Lattice2D(filename, factory, border, v) {}
  ~Lattice2D_reflective() {}

  void NextGeneration() override;
};


void Execute(Lattice &lattice);


#endif // __AUTOMATACELULARGENERAL_HPP__