/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 12: Algoritmos Divide y Vencerás
 * Autor: 
 * Correo: alu0101574042@ull.edu.es
 * Fecha: 03/12/2025
 * Archivo: quickhull_main.cpp - Programa principal
 */

#include "point_set.hpp"
#include "quickhull.hpp"

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

    CyA::point_set ps(points);
    ps.quickHull();

    WriteHullToFile(ps.get_hull(), options.output_filename);

    if (!options.dot_filename.empty()) {
      std::ofstream dot(options.dot_filename);
      ps.write(dot);
    }

  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
