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
 * @file code_structure.cc
 * @brief Fichero que contiene las funciones para el análisis de la estructura
 * del código
 *
 * Historial de versiones
 *   03/10/2025
 *     - Creación (primera versión) del código
 *     - Añadida la clase CodeStructure para representar la estructura del
 * código
 */
#include "../lib/code_structure.h"

#include <iostream>

/**
 * @brief Sobrecarga del operador de salida para la clase CodeStructure
 * @param os - ostream
 * @param structure - Objeto de la clase CodeStructure
 * @return os - ostream modificado
 */
std::ostream& operator<<(std::ostream& os, const CodeStructure& structure) {
  // PROGRAM
  std::string input = structure.GetInputFile();
  std::string program_name = input;
  size_t position = input.find_last_of("/\\");
  if (position != std::string::npos) program_name = input.substr(position + 1);
  os << "PROGRAM: " << program_name << "\n";

  // DESCRIPTION (first DESCRIPTION comment if present)
  os << "DESCRIPTION:" << std::endl;
  bool printed_description = false;
  for (const auto& comment : structure.GetComments()) {
    if (comment.GetType() == CommentType::DESCRIPTION) {
      os << comment.GetContent() << std::endl;
      printed_description = true;
      break;
    }
  }
  if (!printed_description) os << std::endl;

  os << "VARIABLES:" << std::endl;
  for (const auto& variable : structure.GetVariables()) {
    os << variable;
  }

  os << std::endl << "STATEMENTS:" << std::endl;
  for (const auto& loop : structure.GetLoops()) {
    os << loop;
  }

  os << std::endl << "CONDITIONALS:" << std::endl;
  for (const auto& conditional : structure.GetConditionals()) {
    os << conditional;
  }

  os << std::endl << "MAIN:" << std::endl;
  os << (structure.HasMain() ? "True" : "False") << std::endl;

  os << std::endl << "COMMENTS:" << std::endl;
  for (const auto& comment : structure.GetComments()) {
    os << comment;
  }

  os << std::endl << "DIRECTIVAS:" << std::endl;
  for (const auto& d : structure.GetDirectivas()) {
    os << d;
  }
  Directivas::PrintSummary(os);

  return os;
}