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


// Implementation of the operator of access to the coordinates
template <int Dim, class Coordinate_t>
Coor_t PositionDim<Dim, Coordinate_t>::operator[](unsigned int i) const
{
  if (i >= Dim)
    throw ac_exception();
  return Coordinates[i];
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

Lattice1D::Lattice1D(const Position &position, const FactoryCell &factory, const std::string &border, const std::optional<State> &v) : Lattice(position, factory, border, v)
{
  size_ = position[0];
  if (position[0] <= 0)
  {
    std::cout << "Position: " << position[0] << " is out of range\n";
    throw ac_exception();
    exit(1);
  }

  cell1D_ = new Cell *[position[0]];
  for (int i = 0; i < position[0]; i++)
  {
    cell1D_[i] = factory.CreateCell(PositionDim<1>(i), rand() % 2);
  }
}

Lattice1D::Lattice1D(const char *filename, const FactoryCell &factory, const std::string &border, const std::optional<State> &v) : Lattice(filename, factory, border, v) 
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    std::cerr << "Error: File not found" << std::endl;
    exit(1);
  }

  file >> size_;
  cell1D_ = new Cell *[size_];
  std::string line;
  std::getline(file, line);
  std::getline(file, line);
  for (int i = 0; i < size_; i++)
  {
    cell1D_[i] = factory.CreateCell(PositionDim<1>(i), line[i] - '0');
  }
  std::cout << "Line: " << line << "\n"; // "Line:  
  file.close();
}

Lattice1D::~Lattice1D()
{
  for (int i = 0; i < size_; i++)
  {
    delete cell1D_[i];
  }
  delete[] cell1D_;
}

void Lattice1D::NextGeneration()
{
  int size = size_;
  for (int i = 0; i < size; i++)
  {
    cell1D_[i]->NextState(*this, PositionDim<1>(i));
  }
  for (int i = 0; i < size; i++)
  {
    cell1D_[i]->UpdateState();
  }
}

std::size_t Lattice1D::Population() const
{
  int size = size_;
  std::size_t population = 0;
  for (int i = 0; i < size; i++)
  {
    if (cell1D_[i]->GetState() == kAlive)
      population++;
  }
  return population;
}

void Lattice1D::DisplayLattice(std::ostream &os) const
{
  int size = size_;
  os << "| ";
  for (int i = 0; i < size; i++)
  {
    os << *cell1D_[i];
    if (i < size - 1)
      os << " | ";
  }
  os << " |";
  os << std::endl;
}

/**
 * @brief Get cell with periodic border
 * @param position
 * @return const Cell&
 */
const State Lattice1D::PeriodicBoder(const Position &position) const
{
  return cell1D_[(position[0] + size_) % size_]->GetState();
}

/**
 * @brief Get cell with open border
 * @param position
 * @return const Cell&
 */
const State Lattice1D::OpenBoder(const Position &position) const
{
  return (position[0] < 0 || position[0] >= size_) ? v_.value() : cell1D_[position[0]]->GetState();
}

/**
 * @brief Get the state of the cell
 * @param position
 * @return const State
 */
const State Lattice1D::GetCellState(const Position &position) const
{
  return (border_ == "open") ? OpenBoder(position) : PeriodicBoder(position);
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

std::ostream &operator<<(std::ostream &os, const Cell &cell)
{
  cell.DisplayCell(os);
  return os;
}

Lattice2D::Lattice2D(const Position &position, const FactoryCell &factory, const std::string &border, const std::optional<State> &v) : Lattice(position, factory, border, v)
{
  rows_ = position[0];
  cols_ = position[1];
  cell2D_ = new Cell **[rows_];
  for (int i = 0; i < rows_; i++)
  {
    cell2D_[i] = new Cell *[cols_];
    for (int j = 0; j < cols_; j++)
    {
      cell2D_[i][j] = factory_.CreateCell(PositionDim<2>(i, j), rand() % 2);
    }
  }
}

Lattice2D::Lattice2D(const char *filename, const FactoryCell &factory, const std::string &border, const std::optional<State> &v) : Lattice(filename, factory, border, v)
{
  std::cout << "Lattice2D constructor" << std::endl;
  std::ifstream file(filename);
  if (!file.is_open())
  {
    std::cerr << "Error: File not found" << std::endl;
    exit(1);
  }

  file >> rows_ >> cols_;
  PositionDim<2> position(2, rows_, cols_);
  std::string line;
  std::getline(file, line);
  cell2D_ = new Cell **[rows_];
  for (int i = 0; i < rows_; i++)
  {
    cell2D_[i] = new Cell *[cols_];
    std::getline(file, line);
    for (int j = 0; j < cols_; j++)
    {
      State state = (line[j] == '1') ? kAlive : kDead;
      cell2D_[i][j] = factory_.CreateCell(PositionDim<2>(i, j), state);
    }
  }

  file.close();
}

const State Lattice2D::GetCellState(const Position &position) const
{
  if (position[0] < 0 || position[0] >= rows_ || position[1] < 0 || position[1] >= cols_)
  {
    if (border_ == "noborder")
    {
      return NoBoreder(position);
    }
    else if (border_ == "reflective")
    {
      return Reflective(position);
    }
  }
  return cell2D_[position[0]][position[1]]->GetState();
}

const State Lattice2D::NoBoreder(const Position &position) const
{
  return kDead;
}

const State Lattice2D::Reflective(const Position &position) const
{
  int x = ReflectCoordinate(position[0], rows_);
  int y = ReflectCoordinate(position[1], cols_);

  return cell2D_[x][y]->GetState();
}

int Lattice2D::ReflectCoordinate(int coordinate, int dimensionSize) const
{
  if (coordinate < 0)
  {
    return -coordinate;
  }
  else if (coordinate >= dimensionSize)
  {
    return 2 * dimensionSize - coordinate - 2;
  }
  return coordinate;
}


void Lattice2D_noborder::NextGeneration()
{
  // Compute the next state for each cell
  ComputeNextState();
  
  // Update the state of each cell
  UpdateCellStates();
  
  // Identify the positions of next alive cells
  std::set<PositionDim<2>> next_alive_cell;
  IdentifyNextAliveCells(next_alive_cell);
  
  // Push at borders for next alive cells
  PushAtBorders(next_alive_cell);
  
  // Reset expansion flags
  ResetExpansionFlags();
}

void Lattice2D::ComputeNextState()
{
  for (int i = 0; i < rows_; i++)
  {
    for (int j = 0; j < cols_; j++)
    {
      cell2D_[i][j]->NextState(*this, PositionDim<2>(i, j));
    }
  }
}

void Lattice2D::UpdateCellStates()
{
  for (int i = 0; i < rows_; i++)
  {
    for (int j = 0; j < cols_; j++)
    {
      cell2D_[i][j]->UpdateState();
    }
  }
}

void Lattice2D_noborder::IdentifyNextAliveCells(std::set<PositionDim<2>>& next_alive_cell)
{
  for (int i = 0; i < rows_; i++)
  {
    for (int j = 0; j < cols_; j++)
    {
      if (cell2D_[i][j]->GetState() == kAlive)
      {
        next_alive_cell.insert(PositionDim<2>(i, j));
      }
    }
  }
}

void Lattice2D_noborder::PushAtBorders(const std::set<PositionDim<2>>& next_alive_cell)
{
  for (const auto& position : next_alive_cell)
  {
    if (position[0] == 0 || position[0] == rows_ - 1 || position[1] == 0 || position[1] == cols_ - 1)
    {
      Push_at(position);
    }
  }
}

void Lattice2D_noborder::ResetExpansionFlags()
{
  already_expanded_back = false;
  already_expanded_front = false;
  already_expanded_top = false;
  already_expanded_bottom = false;
}


void Lattice2D_reflective::NextGeneration()
{
  ComputeNextState();
  UpdateCellStates();
}

void Lattice2D::CleanLattice2D()
{
  for (int i = 0; i < rows_; i++)
  {
    delete[] cell2D_[i];
  }
  delete[] cell2D_;
}

/**
 * @brief Copy the Lattice2D_noborder
 * @param origen
 * @param destino
 * @param filas
 * @param columnas
 * @return void
 */
void Lattice2D_noborder::CopyLattice2D(Cell ***origen, Cell ***&destino, int filas, int columnas)
{
  destino = new Cell **[filas];
  for (int i = 0; i < filas; i++)
  {
    destino[i] = new Cell*[columnas];
  }
  for (int i = 0; i < std::min(rows_, filas); i++)
  {
    for (int j = 0; j < std::min(cols_, columnas); j++)
    {
      destino[i][j] = origen[i][j];
    }
  }
}

/**
 * @brief Inser a row of cells at the end of the Lattice2D_noborder
 * @param cell
 * @return void
 */
void Lattice2D_noborder::Push_back()
{
  if (already_expanded_back)
  {
    return;
  }
  already_expanded_back = true;

  Cell ***aux_cell;
  CopyLattice2D(cell2D_, aux_cell, rows_, cols_ + 1);
  for (int i = 0; i < rows_; i++)
  {
    aux_cell[i][cols_] = factory_.CreateCell(PositionDim<2>(i, cols_), kDead);
  }
  CleanLattice2D();
  cell2D_ = aux_cell;
  SetCols(cols_ + 1);
}

/**
 * @brief Inser a row of cells at the beginning of the Lattice2D_noborder
 * @param cell
 * @return void
 */
void Lattice2D_noborder::Push_front()
{
  if (already_expanded_front)
  {
    return;
  }
  already_expanded_front = true;

  Cell ***aux_cell;
  CopyLattice2D(cell2D_, aux_cell, rows_, cols_ + 1);
  for (int i = 0; i < rows_; i++)
  {
    for (int j = cols_; j > 0; j--)
    {
      aux_cell[i][j] = aux_cell[i][j - 1];
    }
    aux_cell[i][0] = factory_.CreateCell(PositionDim<2>(i, 0), kDead);
  }
  CleanLattice2D();
  cell2D_ = aux_cell;
  SetCols(cols_ + 1);
}

/**
 * @brief Inser a column of cells at left of the Lattice2D_noborder
 * @param cell
 * @return void
 */
void Lattice2D_noborder::Push_top()
{
  if (already_expanded_top)
  {
    return;
  }
  already_expanded_top = true;

  Cell ***aux_cell;
  CopyLattice2D(cell2D_, aux_cell, rows_ + 1, cols_);
  for (int i = rows_; i > 0; i--)
  {
    for (int j = 0; j < cols_; j++)
    {
      aux_cell[i][j] = aux_cell[i - 1][j];
    }
  }
  for (int i = 0; i < cols_; i++)
  {
    aux_cell[0][i] = factory_.CreateCell(PositionDim<2>(0, i), kDead);
  }
  CleanLattice2D();
  cell2D_ = aux_cell;
  SetRows(rows_ + 1);
}

/**
 * @brief Inser a column of cells at right of the Lattice2D_noborder
 * @param cell
 * @return void
 */
void Lattice2D_noborder::Push_bottom()
{
  if (already_expanded_bottom)
  {
    return;
  }
  already_expanded_bottom = true;

  Cell ***aux_cell;
  CopyLattice2D(cell2D_, aux_cell, rows_ + 1, cols_);
  for (int i = 0; i < cols_; i++)
  {
    aux_cell[rows_][i] = factory_.CreateCell(PositionDim<2>(rows_, i), kDead);
  }
  CleanLattice2D();
  cell2D_ = aux_cell;
  SetRows(rows_ + 1);
}

/**
 * @brief Select Push method
 * @param position
 * @return void
 */
void Lattice2D_noborder::Push_at(const Position &position)
{
  for (int i = 0; i < rows_; i++)
  {
    for (int j = 0; j < cols_; j++)
    {
      if (cell2D_[i][j]->GetState() == kAlive)
      {
        PushCellAtBorder(i, j);
      }
    }
  }
}

void Lattice2D_noborder::PushCellAtBorder(int row, int col)
{
  if (row == 0)
  {
    Push_top();
  }
  if (row == rows_ - 1)
  {
    Push_bottom();
  }
  if (col == 0)
  {
    Push_front();
  }
  if (col == cols_ - 1)
  {
    Push_back();
  }
}

std::size_t Lattice2D::Population() const
{
  std::size_t population = 0;
  for (int i = 0; i < rows_; i++)
  {
    for (int j = 0; j < cols_; j++)
    {
      if (cell2D_[i][j]->GetState() == kAlive)
      {
        population++;
      }
    }
  }
  return population;
}

void Lattice2D::DisplayLattice(std::ostream &os) const
{
  for (int i = 0; i < rows_; i++)
  {
    os << "+" << std::string(cols_ * 4 - 1, '-') << "+" << std::endl;
    os << "| ";
    for (int j = 0; j < cols_; j++)
    {
      os << *cell2D_[i][j];
      os << " | ";
    }
    os << std::endl;
  }
  os << "+" << std::string(cols_ * 4 - 1, '-') << "+" << std::endl;
}

std::ostream &operator<<(std::ostream &os, const Lattice &lattice)
{
  lattice.DisplayLattice(os);
  return os;
}

void Execute(Lattice &lattice)
{
  int generation = 0;
  while (true)
  {
    std::cout << "Generation: " << generation << std::endl;
    std::cout << lattice << std::endl;
    lattice.NextGeneration();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    if (lattice.Population() == 0 || generation++ == 3)
    {
      break;
    }
  }
}

int main()
{
  srand(time(NULL));
  std::cout << "Unidimensional" << std::endl;
  std::string filename1 = "init2.txt";
  FactoryCellACE110 factory2;
  Lattice1D_open lattice2(filename1.c_str(), factory2, "open", kDead);
  Execute(lattice2);

  PositionDim<1> position3(1, 10);
  FactoryCellACE30 factory3;
  std::cout << "CellACE30 periodic" << std::endl;
  Lattice1D_periodic lattice3(position3, factory3, "periodic", kDead);
  Execute(lattice3);


  std::cout << "Bidimensional" << std::endl;
  std::string filename = "init.txt";
  FactoryCellLife23_3 factory1;
  std::cout << "CellLife23_3 noborder" << std::endl;
  Lattice2D_noborder lattice1(filename.c_str(), factory1, "noborder");
  Execute(lattice1);

  PositionDim<2> position4(2, 10, 10);
  FactoryCellLife51_346 factory4;
  std::cout << "CellLife51_346 reflective" << std::endl;
  Lattice2D_reflective lattice4(position4, factory4, "reflective");
  Execute(lattice4);

  return 0;
}
