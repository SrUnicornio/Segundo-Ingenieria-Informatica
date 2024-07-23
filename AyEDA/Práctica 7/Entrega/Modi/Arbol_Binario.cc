/**
 * @file Arbol_Binario
 *
 * @brief Implementación de del TDA Árbol Binario
 *        ABE (Árbol Binario Equilibrado)
 *        ABB (Árbol Binario de Búsqueda)
 *        AVL (Árbol Binario de Búsqueda Balanceado) 
 *
 * Uso de polimorfismo dinámico y sobrecarga de operadores
 *
 * Compilar con g++ -std=c++23 -o Arbol_Binario Arbol_Binario.cc *.h
 */

#include <ctime>
#include <cstdlib>
#include "Albol_Binario.h"


int main(int argc, const char *const argv[])
{
  std::cout << "Práctica 7: Árbol Binario\n";
  std::cout << "===========================\n";

  auto options = parse_args(argc, argv);
  if (!options)
  {
    Usage();
    return 1;
  }

  AB<ROL> *tree = CreateTree(options->tipe, options->showTrace);

  menu1(tree, *options);


  return 0;
}