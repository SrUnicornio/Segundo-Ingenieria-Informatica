#include <iostream>
// numero aleatorio 0 o 1
#include <stdlib.h>
// tiempo
#include <time.h>

int main()
{
  int **tablero = NULL;
  int fila = 5;
  int columna = 5;
  tablero = new int *[fila];
  for (int i = 0; i < fila; i++)
  {
    tablero[i] = new int[columna];
  }
  for (int i = 0; i < fila; i++)
  {
    for (int j = 0; j < columna; j++)
    {
      tablero[i][j] = rand() % 2;
    }
  }
  for (int i = 0; i < fila; i++)
  {
    for (int j = 0; j < columna; j++)
    {
      std::cout << (tablero[i][j] == 1 ? "X" : " ") << " ";
    }
    std::cout << std::endl;
  }

  for (int i = 0; i < fila; i++)
  {
    delete[] tablero[i];
  }
  delete[] tablero;
  return 0;
}