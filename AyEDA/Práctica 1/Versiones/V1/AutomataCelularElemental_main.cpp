/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asigantura: Algoritmos y Estructuras de Datos Avanzadas
 *
 * @date: 03 FEB 2024
 * @author: 
 * @file AutomataCelularElemental_main.cpp
 * @brief Programa principal
  Create a program that simulates the evolution of a one-dimensional cellular automaton. The program will receive the following parameters:
  -size: Size of the lattice. Default value is 1.
  -border: Border type (open or periodic). Default value is open.
  -v: Value of the border cells. (Only for open borders). Default value is 0.
  -init: Initial state of the lattice. If not specified, the initial state will be a single cell in the middle of the lattice with state 1. (Optional)
  In case don't recive '-init' parameter, the program will set the state of the cells in the middle of the lattice to 1 in the case of an even number of cells, and set the state of the cell in the middle of the lattice to 1 in the case of an odd number of cells.
 */

#include "AutomataCelularElemental.hpp"
#include <thread>

int main(int argc, char *argv[])
{
  auto options = parse_args(argc, argv);
  if (!options || options->show_help)
  {
    Usage(argv);
    return options ? 0 : 1;
  }

  Cell *cells = NULL;
  cells = new Cell[options->size];
  Lattice lattice(cells, options->size, options->border, std::optional<State>(options->v));

  if (!options->init.empty())
  {
    lattice.Load(options->init);
  }
  else
  {
    if (options->size % 2 == 0)
    {
      // Set the state of the cells in the middle of the lattice to 1 in the case of an even number of cells
      Cell&cell = const_cast<Cell&>(lattice.GetCell(options->size / 2));
      cell.SetState(1);
      Cell&cell2 = const_cast<Cell&>(lattice.GetCell(options->size / 2 - 1));
      cell2.SetState(1);
    }
    else
    {
      // Set the state of the cell in the middle of the lattice to 1 in the case of an odd number of cells
      Cell&cell = const_cast<Cell&>(lattice.GetCell(options->size / 2));
      cell.SetState(1);
    }
  }
  int cont = 0;
  while (true)
  {
    std::cout << "Generación " << cont << " ";
    std::cout << lattice << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    lattice.NextGeneration();
    if (cont++ == 3)
    {
      break;
    }
  }

  delete cells;

  return 0;
}
