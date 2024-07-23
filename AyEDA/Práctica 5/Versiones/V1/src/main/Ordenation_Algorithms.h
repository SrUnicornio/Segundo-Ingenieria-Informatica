/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * 
 * Algoritmos y Estructuras de Datos
 * Práctica 5: Algoritmos de ordenación
 * 
 * @file Ordenation_Algorithms.h
 * @author 
 * @date 02 MAR 2024
 * @brief Fichero de cabecera para los algoritmos de ordenación
 */

#ifndef __ORDENATION_ALGORITHMS_H__
#define __ORDENATION_ALGORITHMS_H__

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <optional>
#include <fstream>

// static int RUN = 32;

// Class Sequence and StaticSequence
#include "../StaticSequence/StaticSequence.h"

// Class SortMethod
#include "../SortMethod/SortMethod.h"

// Cuadratic sort methods
#include "../SortAlgorithm/QuadraticSorts.h"

// Logarithmic sort methods
#include "../SortAlgorithm/LogarithmSorts.h"

// Other sort methods
#include "../SortAlgorithm/OtherSorts.h"

//Class NIF
#include "../NIF/NIF.h"

struct program_options
{
  unsigned int sequenceSize = 0;
  int sortMethod = 0;
  std::string traceOption;
  std::string init;
  std::string filename;
  bool isManual = false;
  bool isRandom = false;
  bool isFile = false;
};

/**
 * @brief Show the usage of the program
*/
void Usage()
{
  std::cout << "Usage: ./";
  std::cout << "-size <s> : sequence size\n";
  std::cout << "-ord <m> : is the code of the sorting method\n";
  std::cout << "\t m = 1 -> Insertion\n";
  std::cout << "\t m = 2 -> Selection\n";
  std::cout << "\t m = 3 -> Bubble\n";
  std::cout << "\t m = 4 -> Cocktail\n";
  std::cout << "\t m = 5 -> MergeSort\n";
  std::cout << "\t m = 6 -> QuickSort\n";
  std::cout << "\t m = 7 -> HeapSort\n";
  std::cout << "\t m = 8 -> ShellSort\n";
  std::cout << "\t m = 9 -> RadixSort\n";
  std::cout << "\t m = 10 -> TimSort\n";
  std::cout << "-init <i> [f]: is the code of the initialization method\n";
  std::cout << "\t i = manual -> manual initialization\n";
  std::cout << "\t i = random -> random initialization\n";
  std::cout << "\t i = file  f -> initialization from file\n";
  std::cout << "-trace <y|n> : show trace of the sorting algorithm\n";
}

/**
 * @brief Parse the arguments of the program
 * @param argc Number of arguments
 * @param argv Array of arguments
 * @return program_options struct with the options
*/
std::optional<program_options> parse_args(int argc, const char *const argv[])
{
  program_options options;
  std::vector<std::string> args(argv + 1, argv + argc);
  if (args.empty())
  {
    return std::nullopt;
  }

  for (auto it = args.begin(); it != args.end(); ++it)
  {
    if (*it == "-size")
    {
      if (++it == args.end())
      {
        return std::nullopt;
      }
      options.sequenceSize = std::stoi(*it);
    }
    else if (*it == "-ord")
    {
      if (++it == args.end())
      {
        return std::nullopt;
      }
      options.sortMethod = std::stoi(*it);
    }
    else if (*it == "-init")
    {
      if (++it == args.end())
      {
        return std::nullopt;
      }
      options.init = *it;
      if (options.init == "manual")
      {
        options.isManual = true;
      }
      else if (options.init == "random")
      {
        options.isRandom = true;
      }
      else if (options.init == "file")
      {
        options.isFile = true;
        if (++it != args.end())
        {
          options.filename = *it;
        }
        else
        {
          return std::nullopt;
        }
      }
      else
      {
        return std::nullopt;
      }
    }
    else if (*it == "-trace")
    {
      if (++it == args.end())
      {
        return std::nullopt;
      }
      options.traceOption = *it;
      if (options.traceOption == "y" || options.traceOption == "Y")
      {
        showTrace = true;
      }
      else if (options.traceOption == "n" || options.traceOption == "N")
      {
        showTrace = false;
      }
      else
      {
        return std::nullopt;
      }
    }
  }
  return options;
}

/**
 * @brief Print the sequence
 * @param sequence Array of NIF
 * @param n Size of the array
*/
void imprimirSecuencia(const NIF *sequence, int n)
{
  for (int i = 0; i < n; i++)
  {
    std::cout << sequence[i] << " ";
  }
}

/**
 * @brief Initialize the sequence
 * @param sequence Array of NIF
 * @param n Size of the array
 * @param options program_options struct with the options
*/
void initilazingSequence(NIF *sequence, int n, program_options &options)
{
  if (options.init == "manual")
  {
    std::cout << "Introduce " << n << " elements: ";
    for (int i = 0; i < n; i++)
    {
      std::cin >> sequence[i];
    }
  }
  else if (options.init == "random")
  {
    std::cout << "Random sequence: ";
    for (int i = 0; i < n; i++)
    {
      sequence[i] = NIF();
    }
    imprimirSecuencia(sequence, n);
    std::cout << std::endl;
  }
  else if (options.init == "file")
  {
    std::ifstream file(options.filename);
    if (!file)
    {
      std::cerr << "Error opening file" << std::endl;
      exit(1);
    }
    for (int i = 0; i < n; i++)
    {
      file >> sequence[i];
    }
    file.close();
  }
  else
  {
    std::cout << "Opción no válida" << std::endl;
    exit(1);
  }
}

/**
 * @brief Sort the sequence
 * @param sequence Array of NIF
 * @param sequenceObj StaticSequence object
 * @param n Size of the array
 * @param options program_options struct with the options
*/
void sortingTipe(NIF *sequence, StaticSequence<NIF> &sequenceObj, int n, program_options &options)
{
  switch (options.sortMethod)
  {
  case 1:
  {
    InsertionSort<NIF> insertionSort(sequenceObj);
    insertionSort.Sort(sequence, n);
    break;
  }
  case 2:
  {

    SelectionSort<NIF> selectionSort(sequenceObj);
    selectionSort.Sort(sequence, n);
    break;
  }
  case 3:
  {
    BubbleSort<NIF> bubbleSort(sequenceObj);
    bubbleSort.Sort(sequence, n);
    break;
  }

  case 4:
  {
    CocktailSort<NIF> cocktailSort(sequenceObj);
    cocktailSort.Sort(sequence, n);
    break;
  }
  case 5:
  {

    MergeSort<NIF> mergeSort(sequenceObj);
    mergeSort.Sort(sequence, n);
    break;
  }
  case 6:
  {

    QuickSort<NIF> quickSort(sequenceObj);
    quickSort.Sort(sequence, n);
    break;
  }
  case 7:
  {

    HeapSort<NIF> heapSort(sequenceObj);
    heapSort.Sort(sequence, n);
    break;
  }
  case 8:
  {
    double alpha = 0;
    while (alpha <= 0 || alpha >= 1)
    {
      std::cout << "Introduzca el valor de alpha (0 < alpha < 1): ";
      std::cin >> alpha;
    }

    ShellSort<NIF> shellSort(sequenceObj, alpha);
    shellSort.Sort(sequence, n);
    break;
  }
  case 9:
  {

    RadixSort<NIF> radixSort(sequenceObj);
    radixSort.Sort(sequence, n);
    break;
  }
  case 10:
  {
    TimSort<NIF> timSort(sequenceObj);
    timSort.Sort(sequence, n);
    break;
  }
  default:
  {
    std::cout << "Opción no válida" << std::endl;
    break;
  }
  }
}

#endif // __ORDENATION_ALGORITHMS_H__
