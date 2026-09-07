/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 4: Code analyzer
 * 
 * @author 
 * @date 03/10/2025
 * @file main.cc
 * @brief Fichero principal del programa
 *
 * Historial de versiones
 *   03/10/2025
 *     - Creación (primera versión) del código
 *     - Añadida la gestión de argumentos
 */

#include "../lib/funcions_main.h"

int main(int argc, char* argv[]) {
  Arguments args;
  CheckArguments(argc, argv, &args);
  std::string input_file = args.input_file;
  std::string output_file = args.output_file;
  CodeStructure code_analysis(input_file);
  AnalyzeFile(input_file, code_analysis);
  WriteAnalysisToFile(output_file.c_str(), code_analysis);
  return 0;
}
