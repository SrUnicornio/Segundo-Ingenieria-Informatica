#ifndef __CELL_CPP__
#define __CELL_CPP__

#include "State.hpp"
#include "Position.hpp"

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

#endif // __CELL_CPP__