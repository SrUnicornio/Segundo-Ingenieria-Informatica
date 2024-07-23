#ifndef __LATTICE_HPP__
#define __LATTICE_HPP__

#include "Cell.hpp"
#include "FactoryCell.hpp"
#include <optional>
#include <set>

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
  char InitGame();
  void RunGame();
  void SaveLattice(const char *filename);

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
  void IdentifyNextAliveCells(std::set<PositionDim<2>> &next_alive_cell);
  void PushAtBorders(const std::set<PositionDim<2>> &next_alive_cell);
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

#endif // __LATTICE_HPP__