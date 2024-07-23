/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * 
 * Algoritmos y Estructuras de Datos
 * Práctica 5: Algoritmos de ordenación
 * 
 * @file main.cc
 * @author Álvaro Pérez Ramos
 * @date 02 MAR 2024
 * @brief Este programa ordena una secuencia de NIFs mediante distintos algoritmos de ordenación.
 * Algoritmos implementados:
 * - Inserción
 * - Selección
 * - Burbuja | Cocktail
 * - MergeSort
 * - QuickSort
 * - HeapSort
 * - ShellSort
 * - RadixSort
 * - TimSort 
 *
 * Compilar con: 
 * mkdir build
 * cd build
 * cmake ..
 * make
*/

#include "Ordenation_Algorithms.h"

int main(int argc, const char *const argv[])
{
  std::cout << "Práctica 5: Algoritmos de ordenación\n";
  std::cout << "=====================================\n";

  auto options = parse_args(argc, argv);
  if (!options)
  {
    Usage();
    return 1;
  }

  NIF *sequence = new NIF[options->sequenceSize];
  int n = options->sequenceSize;

  std::cout << "Initialazing sequence...\n";
  initilazingSequence(sequence, n, *options);

  for (int i = 1; i <= 10; i++)
  {
    options->sortMethod = i;
    if (i != 8)
    {
      std::cout << "Initial sequence: ";
    }
    StaticSequence<NIF> sequenceObj(sequence, n);
    sortingTipe(sequence, sequenceObj, n, *options);

    std::cout << "Comparisons: " << comparations << " |\t";
    std::cout << " Swaps: " << swaps << std::endl;

    comparations = 0;
    swaps = 0;
  }

  std::cout << "Sorted sequence: ";
  imprimirSecuencia(sequence, n);
  std::cout << std::endl;
  delete[] sequence;


  return 0;
}
