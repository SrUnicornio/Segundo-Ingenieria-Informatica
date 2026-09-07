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
 * Archivo: EMST.hpp - Funciones auxiliares
 */

#ifndef EMST_HPP__
#define EMST_HPP__

#include <string>

#include "point_set.hpp"
#include "point_types.hpp"

/// Estructura para almacenar las opciones del programa.
struct ProgramOptions {
  std::string input_filename;
  std::string output_filename;
  std::string dot_filename;
  bool show_help = false;
};

/// Imprime la ayuda del programa.
void PrintUsage();

/// Procesa los argumentos tradicionales.
/// Devuelve true si son válidos.
bool ParseArguments(int argc, char* argv[], ProgramOptions& options);

/// Lee puntos desde un fichero de texto.
CyA::point_vector ReadPointsFromFile(const std::string& filename);

/// Escribe un árbol generador a un fichero.
void WriteTreeToFile(const point_set& tree, const std::string& filename);

#endif
