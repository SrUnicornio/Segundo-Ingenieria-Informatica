/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 12: Algoritmos divide y vencerás
 * Autor: 
 * Correo: alu0101574042@ull.edu.es
 * Fecha: 03/12/2025
 * Archivo:
 */

#include <sstream>

#include "quickhull.hpp"

void PrintUsage() {
  std::cout << "Uso: quickhull [opciones] <input> <output>\n\n"
            << "Opciones:\n"
            << "  -h, --help        Muestra esta ayuda\n"
            << "  -d <dot_file>     Genera archivo DOT\n\n";
}

bool ParseArguments(int argc, char* argv[], ProgramOptions& options) {
  if (argc == 1) return false;

  int i = 1;
  while (i < argc) {
    std::string arg = argv[i];

    if (arg == "-h" || arg == "--help") {
      options.show_help = true;
      return true;
    } else if (arg == "-d") {
      if (i + 1 >= argc) {
        std::cerr << "Error: falta nombre para -d\n";
        return false;
      }
      options.dot_filename = argv[++i];
    } else {
      if (options.input_filename.empty())
        options.input_filename = arg;
      else if (options.output_filename.empty())
        options.output_filename = arg;
      else {
        std::cerr << "Error: demasiados argumentos\n";
        return false;
      }
    }
    ++i;
  }

  return !(options.input_filename.empty() || options.output_filename.empty());
}

CyA::point_vector ReadPointsFromFile(const std::string& filename) {
  std::ifstream input(filename);
  if (!input) throw std::runtime_error("No se pudo abrir " + filename);

  CyA::point_vector pts;
  input >> pts;
  return pts;
}

void WriteHullToFile(const CyA::point_vector& hull,
                     const std::string& filename) {
  std::ofstream out(filename);
  if (!out) throw std::runtime_error("No se pudo abrir salida " + filename);

  out << hull.size() << "\n";
  for (auto& p : hull) out << p.first << " " << p.second << "\n";
}
