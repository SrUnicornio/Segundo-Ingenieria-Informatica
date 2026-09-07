/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 2: Cadenas y lenguajes
 * 
 * @author 
 * @date 17/09/2025
 * @file main.cc
 * @brief Fichero que contiene la función main del programa.
 * 
 * Historial de versiones
 *   17/09/2025
 *     - Creación (primera versión) del código
 *     - Implementación de comprobación de argumentos
 *     - Implementación de las opciones del programa
 */

#include "../lib/main_functions.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
  Arguments args;
  CheckArguments(argc, argv, &args);
  std::string input_file{args.input_file};
  std::string output_file{args.output_file};
  int opcode = args.opcode;

  // Fichero de entrada del estilo cadena alfabeto
  std::vector<InputLine> entradas = ReadInputFile(input_file, output_file);
  std::ofstream file_out{output_file};

  // ShowInputFile(entradas, file_out); // Descomentar para mostrar el contenido del fichero de entrada

  ProcessOptions(entradas, output_file, opcode);
  file_out.close();
  return 0;
}
