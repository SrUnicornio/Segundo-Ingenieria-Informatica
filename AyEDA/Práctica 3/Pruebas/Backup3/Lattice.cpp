#include "Lattice.hpp"
#include <fstream>
#include <thread>

char Lattice::InitGame()
{
  system("clear");
  std::cout << *this;
  std::cout << "Enter command ('x' to exit, 'n' for next generation, 'L' for next 5 generations, 'c' for population count, 's' to save): ";
  char command;
  std::cin >> command;
  return tolower(command);
}

void Lattice::RunGame()
{
  int count_generations = 0;
  while (true)
  {
    char command = InitGame();
    switch (command)
    {
    case 'x':
      system("clear");
      return; // Exit the loop and end the program
    case 'n':
      NextGeneration();
      count_generations++;
      break;
    case 'l':
      for (int i = 0; i < 5; i++)
      {
        NextGeneration();
        count_generations++;
      }
      break;
    case 'c':
      std::cout << "Population: " << Population() << std::endl;

      std::cout << "Press enter to continue...";
      std::cin.ignore();
      std::cin.get();
      break;
    case 's':
    {
      std::string filename = "board of generation" + std::to_string(count_generations) + ".txt";
      SaveLattice(filename.c_str());
      break;
    }
    default:
      std::cerr << "Invalid command. Please try again." << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

/**
 * @brief Saves the Board to a file
 * @param filename
 * @return void
 */
void Lattice::SaveLattice(const char *filename)
{
  std::ofstream output(filename);
  output << *this;
  output.close();
  std::cout << "Board saved to file." << std::endl;
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
  int dimension;
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

/// 2D Lattice

static bool already_expanded_back = false;
static bool already_expanded_front = false;
static bool already_expanded_top = false;
static bool already_expanded_bottom = false;

/**
 * @brief Get the state of the cell
 * @param position
 * @return const State
 */
const State Lattice1D::GetCellState(const Position &position) const
{
  return (border_ == "open") ? OpenBoder(position) : PeriodicBoder(position);
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

void Lattice2D_noborder::IdentifyNextAliveCells(std::set<PositionDim<2>> &next_alive_cell)
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

void Lattice2D_noborder::PushAtBorders(const std::set<PositionDim<2>> &next_alive_cell)
{
  for (const auto &position : next_alive_cell)
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
    destino[i] = new Cell *[columnas];
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
    std::cout << "Pushing at top\n";
    int cont = 0;
    while (cont < row)
    {
      Push_top();
      cont++;
      already_expanded_top = false;
    }
  }
  if (row == rows_ - 1)
  {
    int cont = 0;
    while (cont < row)
    {
      Push_bottom();
      cont++;
      already_expanded_bottom = false;
    }
  }
  if (col == 0)
  {
    int cont = 0;
    while (cont < col)
    {
      Push_front();
      cont++;
      already_expanded_front = false;
    }
  }
  if (col == cols_ - 1)
  {
    int cont = 0;
    while (cont < col)
    {
      Push_back();
      cont++;
      already_expanded_back = false;
    }
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
