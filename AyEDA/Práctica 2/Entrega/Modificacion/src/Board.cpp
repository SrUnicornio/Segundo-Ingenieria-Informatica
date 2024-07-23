#include "../include/Board.hpp"
#include <set>

static bool already_expanded_back = false;
static bool already_expanded_front = false;
static bool already_expanded_top = false;
static bool already_expanded_bottom = false;

/**
 * @brief Constructor of the Board
 * @param data Array of cells
 * @param size Size of the Board
 * @param border Border of the Board
 * @param v Optional state
 * @return void
 */
Board::Board(Cell *cell[], int filas, int columnas, const std::string &border, const std::optional<State> v) : filas_(filas), columnas_(columnas), border_(border), v_(v)
{
  assert(filas > 0 && columnas > 0);
  assert(border == "reflective" || border == "periodic" || border == "open" || border == "noborder");

  cell_ = new Cell *[filas_];
  for (int i = 0; i < filas_; i++)
  {
    cell_[i] = new Cell[columnas_];
    for (int j = 0; j < columnas_; j++)
    {
      cell_[i][j] = cell[i][j];
    }
  }
}

/**
 * @brief Constructor of the Board
 * @param filename
 * @return void
 */
Board::Board(const char *filename, const std::string &border, const std::optional<State> v) : border_(border), v_(v)
{
  std::ifstream input(filename);
  if (!input.is_open())
  {
    std::cerr << "Error al abrir el fichero " << filename << std::endl;
    exit(EXIT_FAILURE);
  }
  input >> filas_;
  input >> columnas_;
  std::cout << "filas: " << filas_ << " columnas: " << columnas_ << std::endl;
  Cell **cell = NULL;
  cell = new Cell *[filas_];
  for (int i = 0; i < filas_; i++)
  {
    cell[i] = new Cell[columnas_];
  }

  std::string line;
  std::getline(input, line);
  for (int i = 0; i < filas_; i++)
  {
    std::getline(input, line);
    for (int j = 0; j < columnas_; j++)
    {
      cell[i][j].SetState(line[j] == '1' ? kAlive : kDead);
    }
  }
  cell_ = cell;
  input.close();
}

/**
 * @brief Copy the lattice
 * @param origen
 * @param destino
 * @param filas
 * @param columnas
 * @return void
 */
void Board::CopyBoard(Cell **origen, Cell **&destino, int filas, int columnas)
{
  destino = new Cell *[filas];
  for (int i = 0; i < filas; i++)
  {
    destino[i] = new Cell[columnas];
  }
  for (int i = 0; i < std::min(filas_, filas); i++)
  {
    for (int j = 0; j < std::min(columnas_, columnas); j++)
    {
      destino[i][j] = origen[i][j];
    }
  }
}

/**
 * @brief Inser a row of cells at the end of the lattice
 * @param cell
 * @return void
 */
void Board::Push_back()
{
  if (already_expanded_back)
  {
    return;
  }
  already_expanded_back = true;

  Cell **aux_cell;
  CopyBoard(cell_, aux_cell, filas_, columnas_ + 1);
  for (int i = 0; i < filas_; i++)
  {
    aux_cell[i][columnas_].SetState(kDead);
  }
  CleanBoard();
  cell_ = aux_cell;
  SetColumnas(columnas_ + 1);
}

/**
 * @brief Inser a row of cells at the beginning of the lattice
 * @param cell
 * @return void
 */
void Board::Push_front()
{
  if (already_expanded_front)
  {
    return;
  }
  already_expanded_front = true;

  Cell **aux_cell;
  CopyBoard(cell_, aux_cell, filas_, columnas_ + 1);
  for (int i = 0; i < filas_; i++)
  {
    for (int j = columnas_; j > 0; j--)
    {
      aux_cell[i][j] = aux_cell[i][j - 1];
    }
    aux_cell[i][0].SetState(kDead);
  }
  CleanBoard();
  cell_ = aux_cell;
  SetColumnas(columnas_ + 1);
}

/**
 * @brief Inser a column of cells at left of the lattice
 * @param cell
 * @return void
 */
void Board::Push_top()
{
  if (already_expanded_top)
  {
    return;
  }
  already_expanded_top = true;

  Cell **aux_cell;
  CopyBoard(cell_, aux_cell, filas_ + 1, columnas_);
  for (int i = filas_; i > 0; i--)
  {
    for (int j = 0; j < columnas_; j++)
    {
      aux_cell[i][j] = aux_cell[i - 1][j];
    }
  }
  for (int i = 0; i < columnas_; i++)
  {
    aux_cell[0][i].SetState(kDead);
  }
  CleanBoard();
  cell_ = aux_cell;
  SetFilas(filas_ + 1);
}

/**
 * @brief Inser a column of cells at right of the lattice
 * @param cell
 * @return void
 */
void Board::Push_bottom()
{
  if (already_expanded_bottom)
  {
    return;
  }
  already_expanded_bottom = true;

  Cell **aux_cell;
  CopyBoard(cell_, aux_cell, filas_ + 1, columnas_);
  for (int i = 0; i < columnas_; i++)
  {
    aux_cell[filas_][i].SetState(kDead);
  }
  CleanBoard();
  cell_ = aux_cell;
  SetFilas(filas_ + 1);
}

/**
 * @brief Select Push method
 * @param position
 * @return void
 */
void Board::Push_at(const Position &position)
{
  if (position.first == 0)
  {
    // std::cout << "Push_top" << std::endl;
    Push_top();
  }
  if (position.first == filas_ - 1)
  {
    // std::cout << "Push_bottom" << std::endl;
    Push_bottom();
  }
  if (position.second == 0)
  {
    // std::cout << "Push_front" << std::endl;
    Push_front();
  }
  if (position.second == columnas_ - 1)
  {
    // std::cout << "Push_back" << std::endl;
    Push_back();
  }

  // redimensionar la matriz cuando se detecte que hay una célula viva en el borde en la siguiente generación
  if (position.first == 0 || position.first == filas_ - 1 || position.second == 0 || position.second == columnas_ - 1)
  {
    for (int i = 0; i < filas_; i++)
    {
      for (int j = 0; j < columnas_; j++)
      {
        if (cell_[i][j].GetState() == kAlive)
        {
          Push_at(std::make_pair(i, j));
        }
      }
    }
  }
}

/**
 * @brief Destructor of the Board
 * @return void
 */
void Board::CleanBoard()
{
  for (int i = 0; i < filas_; i++)
  {
    delete[] cell_[i];
  }
  delete[] cell_;
}

/**
 * @brief Returns the cell at the given position
 * @param position
 * @return Cell at the given position
 */
State Board::GetCellState(const Position &position) const
{
  if (position.first < 0 || position.first >= filas_ || position.second < 0 || position.second >= columnas_)
  {
    if (border_ == "reflective")
    {
      return GetReflectiveState(position);
    }
    else if (border_ == "periodic")
    {
      return GetPeriodicState(position);
    }
    else if (border_ == "open")
    {
      return GetOpenState(position);
    }
    else if (border_ == "noborder")
    {
      return GetNoBordeState(position);
    }
  }
  return cell_[position.first][position.second].GetState();
}

/**
 * @brief Get state of the cell outside the Board with reflective border
 * @param position
 * @return const State& State of the cell
 */
const State Board::GetReflectiveState(const Position &position) const
{
  int i = position.first;
  int j = position.second;
  if (i < 0)
  {
    i = -i;
  }
  else if (i >= filas_)
  {
    i = 2 * filas_ - i - 1;
  }
  if (j < 0)
  {
    j = -j;
  }
  else if (j >= columnas_)
  {
    j = 2 * columnas_ - j - 1;
  }
  return cell_[i][j].GetState();
}

/**
 * @brief Get state of the cell outside the Board with periodic border
 * @param position
 * @return const State& State of the cell
 */
const State Board::GetPeriodicState(const Position &position) const
{
  int i = (position.first + filas_) % filas_;
  int j = (position.second + columnas_) % columnas_;
  return cell_[i][j].GetState();
}

/**
 * @brief Get state of the cell outside the Board with open border
 * @param position
 * @return const State& State of the cell
 */
const State Board::GetOpenState(const Position &position) const
{
  return v_.value();
}

/**
 * @brief Get state of the cell outside the Board with no border
 * @param position
 * @return const State& State of the cell
 */
const State Board::GetNoBordeState(const Position &position) const
{
  return kDead;
}

/**
 * @brief Returns the population of the Board
 * @return Population of the Board
 */
std::size_t Board::Population() const
{
  std::size_t population = 0;
  for (int i = 0; i < filas_; i++)
  {
    for (int j = 0; j < columnas_; j++)
    {
      if (cell_[i][j].GetState() == kAlive)
      {
        population++;
      }
    }
  }
  return population;
}

/**
 * @brief Calculates the next state of the cell
 */
/**
 * @brief Calculates the next state of the cell
 */
void Board::NextGeneration()
{
  std::set<Position> next_alive_cells;

  for (int i = 0; i < filas_; i++)
  {
    for (int j = 0; j < columnas_; j++)
    {
      cell_[i][j].NextState(*this, std::make_pair(i, j));
    }
  }
  for (int i = 0; i < filas_; i++)
  {
    for (int j = 0; j < columnas_; j++)
    {
      cell_[i][j].UpdateState();
    }
  }
  if (border_ == "noborder")
  {
    for (int i = 0; i < filas_; i++)
    {
      for (int j = 0; j < columnas_; j++)
      {
        if (cell_[i][j].GetState() == kAlive)
        {
          next_alive_cells.insert(std::make_pair(i, j));
        }
      }
    }

    // redimensionar la matriz cuando se detecte que hay una célula viva en el borde en la siguiente generación
    for (auto it = next_alive_cells.begin(); it != next_alive_cells.end(); ++it)
    {
      Push_at(*it);
    }
    already_expanded_back = false;
    already_expanded_front = false;
    already_expanded_top = false;
    already_expanded_bottom = false;
  }
}

char Board::InitGame()
{
  system("clear");
  std::cout << *this;
  std::cout << "Enter command ('x' to exit, 'n' for next generation, 'L' for next 5 generations, 'c' for population count, 's' to save): ";
  char command;
  std::cin >> command;
  return tolower(command);
}

/**
 * @brief Runs the game
 * @return void
 */
void Board::RunGame()
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
      SaveBoard(filename.c_str());
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
void Board::SaveBoard(const char *filename)
{
  std::ofstream output(filename);
  output << *this;
  output.close();
  std::cout << "Board saved to file." << std::endl;
}

/**
 * @brief Overload of the insertion operator in flow
 * The Board is responsible for its display on the screen, which it does by overloading the insertion operator in flow. To facilitate the display, the character 'X' will be used to represent the state value "1", and the space character ' ' for the state value "0".
 * @param os
 * @param Board
 * @return States of the cells
 */
std::ostream &operator<<(std::ostream &os, const Board &Board)
{
  for (int i = 0; i < Board.GetFilas(); i++)
  {
    // Poner un borde superior
    os << "+" << std::string(Board.GetColumnas() * 4 - 1, '-') << "+" << std::endl;
    os << "| ";
    for (int j = 0; j < Board.GetColumnas(); j++)
    {

      os << Board.GetCell(std::make_pair(i, j)) << " | ";
    }

    os << std::endl;
  }
  os << "+" << std::string(Board.GetColumnas() * 4 - 1, '-') << "+" << std::endl;
  return os;
}
