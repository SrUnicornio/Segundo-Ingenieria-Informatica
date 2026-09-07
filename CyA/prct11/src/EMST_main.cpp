/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 11: Algoritmos Voraces
 * Autor: 
 * Correo: alu0101574042@ull.edu.es
 * Fecha: 03/12/2025
 * Archivo: EMST_main.cpp - Programa principal
 */

#include "EMST.hpp"

int main(int argc, char* argv[]) {
  ProgramOptions options;

  if (!ParseArguments(argc, argv, options)) {
    PrintUsage();
    return EXIT_FAILURE;
  }

  if (options.show_help) {
    PrintUsage();
    return EXIT_SUCCESS;
  }

  try {
    CyA::point_vector points = ReadPointsFromFile(options.input_filename);

    point_set tree(points);
    tree.EMST();

    WriteTreeToFile(tree, options.output_filename);

    if (!options.dot_filename.empty()) {
      WriteTreeToFile(tree, options.dot_filename);
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
