/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 4: Code analyzer
 * @author 
 * @date 03/10/2025
 * @file conditionals.h
 * @brief Fichero que contiene las funciones para el análisis de condicionales
 *
 * Historial de versiones
 *   03/10/2025
 *     - Creación (primera versión) del código
 *     - Añadida la clase Conditional para representar condicionales en el
 * código
 *     - Añadida la función estática Match para detectar declaraciones de
 * condicionales
 *     - Sobrecarga del operador de salida para la clase Conditional
 */

#include "../lib/conditionals.h"

#include <regex>

/**
 * @brief Función estática para detectar declaraciones de condicionales en una línea de código
 * @param line Línea de código a analizar
 * @param result Objeto Conditional que almacenará el resultado
 * @param line_number Número de línea del código
 * @return true si se encuentra un condicional, false en caso contrario
 */
bool Conditional::Match(const std::string& line, Conditional& result,
                        int line_number) {
  static const std::regex if_regex(R"(\bif\s*\()");
  static const std::regex else_if_regex(R"(\belse\s+if\s*\()");
  static const std::regex else_regex(R"(\belse\b)");
  static const std::regex switch_regex(R"(\bswitch\s*\()");

  if (std::regex_search(line, else_if_regex)) {
    result = Conditional(ConditionalType::ELSE_IF, line_number);
    return true;
  } else if (std::regex_search(line, if_regex)) {
    result = Conditional(ConditionalType::IF, line_number);
    return true;
  } else if (std::regex_search(line, else_regex)) {
    result = Conditional(ConditionalType::ELSE, line_number);
    return true;
  } else if (std::regex_search(line, switch_regex)) {
    result = Conditional(ConditionalType::SWITCH, line_number);
    return true;
  }
  return false;
}

/**
 * @brief Sobrecarga del operador de salida para la clase Conditional
 * @param os - ostream 
 * @param cond - Objeto de la clase Conditional
 * @return os - ostream modificado
 */
std::ostream& operator<<(std::ostream& os, const Conditional& cond) {
  std::string type;
  switch (cond.type_) {
    case ConditionalType::IF:
      type = "if";
      break;
    case ConditionalType::ELSE_IF:
      type = "else if";
      break;
    case ConditionalType::ELSE:
      type = "else";
      break;
    case ConditionalType::SWITCH:
      type = "switch";
      break;
  }
  os << "[Line " << cond.line_ << "] " << type << std::endl;
  return os;
}
