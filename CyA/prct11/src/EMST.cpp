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
 * Archivo: EMST.cpp - Funciones auxiliares
 */

#include "EMST.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

/**
 * @brief Imprime la ayuda del programa.
 */
void PrintUsage() {
  std::cout << "Uso: emst [opciones] <fichero_entrada> <fichero_salida>\n\n"
            << "Opciones:\n"
            << "  -h, --help        Muestra esta ayuda\n"
            << "  -d <dot_file>     Genera un archivo DOT con el árbol\n\n"
            << "Ejemplo:\n"
            << "  ./emst puntos.txt salida.txt -d arbol.dot\n";
}

/**
 * @brief Procesa los argumentos tradicionales.
 * @param argc - Número de argumentos.
 * @param argv - Vector de argumentos.
 * @param options - Estructura para almacenar las opciones del programa.
 * @return true si los argumentos son válidos, false en caso contrario.
 */
bool ParseArguments(int argc, char* argv[], ProgramOptions& options) {
  if (argc == 1) return false;

  int i = 1;
  while (i < argc) {
    std::string arg = argv[i];

    if (arg == "-h" || arg == "--help") {
      options.show_help = true;
      return true;
    }
    else if (arg == "-d" || arg == "--dot") {
      if (i + 1 >= argc) {
        std::cerr << "Error: falta el nombre del archivo para -d\n";
        return false;
      }
      options.dot_filename = argv[++i];
    }
    else {
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

  /// Comprobación final
  if (!options.show_help &&
      (options.input_filename.empty() || options.output_filename.empty())) {
    std::cerr << "Error: faltan archivo de entrada o salida\n";
    return false;
  }

  return true;
}

/**
 * @brief Lee puntos desde un fichero de texto.
 * @param filename - Nombre del archivo de entrada.
 * @return Vector de puntos leídos.
 */
CyA::point_vector ReadPointsFromFile(const std::string& filename) {
  std::ifstream input(filename);
  if (!input) {
    throw std::runtime_error("No se pudo abrir el archivo: " + filename);
  }

  CyA::point_vector points;
  std::stringstream buffer;
  std::string line;

  while (std::getline(input, line)) {
    if (!line.empty() && line[0] != '#') buffer << line << "\n";
  }

  buffer >> points;
  return points;
}

/**
 * @brief Escribe un árbol generador a un fichero.
 * @param tree - Árbol generador.
 * @param filename - Nombre del archivo de salida.
 */
void WriteTreeToFile(const point_set& tree, const std::string& filename) {
  std::ofstream output(filename);
  if (!output) {
    throw std::runtime_error("No se pudo abrir archivo de salida: " + filename);
  }
  tree.write(output);
}
