#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <thread>

typedef int State;
const State ALIVE = 1;
const State DEAD = 0;
typedef int PositionN;
typedef int PositionM;

class Tablero;
class Cell
{
 public:
  Cell() {}
  Cell(const PositionN &n, const PositionM &m, const State &state = DEAD) : n_(n), m_(m), state_(state) {}

  State GetState() const { return state_; }
  State SetState(State);

  int NextState(const Tablero &, const PositionN &, const PositionM &);
  void UpdateState() { state_ = SetState(nextstate_); }

  friend std::ostream &operator<<(std::ostream &, const Cell &);

 private:
  PositionN n_;
  PositionM m_;
  State state_;
  State nextstate_;
};

class Tablero
{
 public:
  Tablero() {}
  Tablero(Cell **cell[], const int &fila, const int &columna) : fila_(fila), columna_(columna) { cell_ = *cell; }
  ~Tablero()
  {
    for (int i = 0; i < fila_; i++)
    {
      delete[] cell_[i];
    }
    delete[] cell_;
  }

  int GetFila() const { return fila_; }
  int GetColumna() const { return columna_; }

  const Cell &GetCell(const PositionN &n, const PositionM &m) const { return cell_[n][m]; }
  void NextGeneration();

  friend std::ostream &operator<<(std::ostream &, const Tablero &);

 private:
  int fila_;
  int columna_;
  Cell **cell_;
};

State Cell::SetState(State state)
{
  state_ = state;
  return state;
}

int Cell::NextState(const Tablero &tablero, const PositionN &fila, const PositionM &columna)
{
  int cont = 0;
  State nextstate;

  for (int i = std::max(fila - 1, 0); i <= std::min(fila + 1, tablero.GetFila() - 1); i++)
  {
    for (int j = std::max(columna - 1, 0); j <= std::min(columna + 1, tablero.GetColumna() - 1); j++)
    {
      if (!(i == fila && j == columna)) // No contabilizar la celda actual
      {
        if (tablero.GetCell(i, j).GetState() == ALIVE)
        {
          cont++;
        }
      }
    }
  }
  switch (cont)
  {
  case 2:
    nextstate = state_;
    break;
  case 3:
    nextstate = ALIVE;
    break;
  default:
    nextstate = DEAD;
    break;
  }
  nextstate_ = nextstate;
  return nextstate;
}

void Tablero::NextGeneration()
{
  for (int i = 0; i < fila_; i++)
  {
    for (int j = 0; j < columna_; j++)
    {
      cell_[i][j].NextState(*this, i, j);
    }
  }
  for (int i = 0; i < fila_; i++)
  {
    for (int j = 0; j < columna_; j++)
    {
      cell_[i][j].UpdateState();
    }
  }
}

std::ostream &operator<<(std::ostream &os, const Cell &cell)
{
  os << (cell.GetState() == ALIVE ? "X" : " ");
  return os;
}

std::ostream &operator<<(std::ostream &os, const Tablero &tablero)
{
  for (int i = 0; i < tablero.GetFila(); i++)
  {
    for (int j = 0; j < tablero.GetColumna(); j++)
    {
      os << tablero.GetCell(i,j);
    }
    os << std::endl;
  }
  return os;
}


int main(int argc, char *argv[])
{
  srand(time(NULL));
  Cell **cell;
  int fila = atoi(argv[1]);
  int columna = atoi(argv[2]);
  cell = new Cell *[fila];
  for (int i = 0; i < fila; i++)
  {
    cell[i] = new Cell[columna];
  }
  for (int i = 0; i < fila; i++)
  {
    for (int j = 0; j < columna; j++)
    {
      cell[i][j] = Cell(i, j, rand() % 2);
    }
  }
  Tablero tablero(&cell, fila, columna);
  int i = 0;
  while (true)
  {
    std::cout << "Generacion: " << i << std::endl;
    std::cout << tablero;
    tablero.NextGeneration();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    if (i++ == 4)
    {
      break;
    }
  }
  return 0;
}