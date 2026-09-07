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
 * @file loops.cc
 * @brief Fichero que contiene las funciones para el análisis de bucles
 *
 * Historial de versiones
 *   03/10/2025
 *     - Creación (primera versión) del código
 *     - Añadida la clase Loop para representar bucles en el código
 */
#include "../lib/loops.h"

#include <regex>

/**
 * @brief Función estática para detectar declaraciones de bucles en una línea de
 * código
 * @param line Línea de código a analizar
 * @param result Objeto Loop que almacenará el resultado
 * @param line_number Número de línea del código
 * @return true si se encuentra un bucle, false en caso contrario
 */
bool Loop::Match(const std::string& line, Loop& result, int line_number) {
  static const std::regex for_regex(R"(\bfor\s*\(([^;]*);)");
  static const std::regex while_regex(R"(\bwhile\s*\([^)]*\))");
  static const std::regex do_while_head_regex(R"(\bdo\s*\{)");
  static const std::regex do_while_tail_regex(R"(\bwhile\s*\([^)]*\)\s*;)");

  // Prioritize 'do' detection so we capture the start of do-while blocks.
  if (std::regex_search(line, do_while_head_regex)) {
    result = Loop(LoopType::DO_WHILE, line_number);
    return true;
  }

  // 'for' loops
  if (std::regex_search(line, for_regex)) {
    result = Loop(LoopType::FOR, line_number);
    return true;
  }

  // 'while' loops (but not do-while tails)
  if (std::regex_search(line, while_regex)) {
    if (std::regex_search(line, do_while_tail_regex)) {
      // This looks like a do-while tail; do not report it as a standalone
      // WHILE.
      return false;
    }
    result = Loop(LoopType::WHILE, line_number);
    return true;
  }
  return false;
}

/**
 * @brief Sobrecarga del operador de salida para la clase Loop
 * @param os - ostream
 * @param loop - Objeto de la clase Loop
 * @return os - ostream modificado
 */
std::ostream& operator<<(std::ostream& os, const Loop& loop) {
  std::string type;
  switch (loop.type_) {
    case LoopType::FOR:
      type = "for";
      break;
    case LoopType::WHILE:
      type = "while";
      break;
    case LoopType::DO_WHILE:
      type = "do while";
      break;
  }
  os << "[Line " << loop.line_ << "] " << type << std::endl;
  return os;
}