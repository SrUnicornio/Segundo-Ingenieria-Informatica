#include <algorithm>
#include <iostream>
#include <stdlib.h>
#include <time.h>

class Tablero
{
public:
  Tablero(int fila, int columna);
  ~Tablero();

  int getFila() { return fila; }
  int getColumna() { return columna; }

  void Push_back();
  void Push_front();
  void Push_top();
  void Push_bottom();
  friend std::ostream &operator<<(std::ostream &os, Tablero &tablero);

private:
  int **tablero;
  int fila;
  int columna;

  void CopiarTablero(int **origen, int **&destino, int nuevaFila, int nuevaColumna);
};

Tablero::Tablero(int fila, int columna)
{
  this->fila = fila;
  this->columna = columna;
  tablero = new int *[fila];
  for (int i = 0; i < fila; i++)
  {
    tablero[i] = new int[columna];
    for (int j = 0; j < columna; j++)
    {
      tablero[i][j] = rand() % 2;
    }
  }
}

Tablero::~Tablero()
{
  for (int i = 0; i < fila; i++)
  {
    delete[] tablero[i];
  }
  delete[] tablero;
}

void Tablero::CopiarTablero(int **origen, int **&destino, int nuevaFila, int nuevaColumna)
{
  destino = new int *[nuevaFila];
  for (int i = 0; i < nuevaFila; i++)
  {
    destino[i] = new int[nuevaColumna];
  }
  for (int i = 0; i < std::min(fila, nuevaFila); i++)
  {
    for (int j = 0; j < std::min(columna, nuevaColumna); j++)
    {
      destino[i][j] = origen[i][j];
    }
  }
}

void Tablero::Push_back()
{
  int **aux;
  CopiarTablero(tablero, aux, fila, columna + 1);
  for (int i = 0; i < fila; i++)
  {
    aux[i][columna] = 0; // Llenar la nueva columna con ceros
  }
  delete[] tablero;
  tablero = aux;
  columna++;
}

void Tablero::Push_front()
{
  int **aux;
  CopiarTablero(tablero, aux, fila, columna + 1);
  for (int i = 0; i < fila; i++)
  {
    for (int j = columna; j > 0; j--)
    {
      aux[i][j] = aux[i][j - 1];
    }
    aux[i][0] = 0; // Llenar la primera columna con ceros
  }
  delete[] tablero;
  tablero = aux;
  columna++;
}

void Tablero::Push_top()
{
  int **aux;
  CopiarTablero(tablero, aux, fila + 1, columna);
  for (int i = fila; i > 0; i--)
  {
    for (int j = 0; j < columna; j++)
    {
      aux[i][j] = aux[i - 1][j];
    }
  }
  delete[] tablero;
  tablero = aux;
  fila++;
}

void Tablero::Push_bottom()
{
  int **aux;
  CopiarTablero(tablero, aux, fila + 1, columna);
  for (int i = fila; i > 0; i--)
  {
    for (int j = 0; j < columna; j++)
    {
      aux[i][j] = aux[i - 1][j];
    }
  }
  delete[] tablero;
  tablero = aux;
  fila++;
}

std::ostream &operator<<(std::ostream &os, Tablero &tablero)
{
  for (int i = 0; i < tablero.fila; i++)
  {
    os << "+" << std::string(tablero.columna * 4 - 1, '-') << "+" << std::endl;
    os << "| ";
    for (int j = 0; j < tablero.columna; j++)
    {
      os << (tablero.tablero[i][j] == 0 ? " " : "X");
      os << " | ";
    }
    os << std::endl;
  }
  os << "+" << std::string(tablero.columna * 4 - 1, '-') << "+" << std::endl;
  return os;
}

int main()
{
  srand(time(NULL));
  Tablero tablero(5, 5);
  std::cout << tablero;
  tablero.Push_back();
  std::cout << tablero;
  tablero.Push_front();
  std::cout << tablero;
  tablero.Push_top();
  std::cout << tablero;
  tablero.Push_bottom();
  std::cout << tablero;
  return 0;
}
