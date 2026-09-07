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
 * @file alphabet.cc
 * @brief Fichero que contiene la declaración de la clase Alphabet
 * 
 * Historial de versiones
 *   17/09/2025
 *     - Creación (primera versión) del código 
 *     - Cambio de std::vector a std::set para evitar símbolos repetidos
 *     - Sobrecarga de operadores y control de errores
 *     - Añadidos métodos para insertar y eliminar símbolos
 */

#include "../lib/alphabet.h"

/**
 * @brief Constructor de la clase Alphabet.
 * @param symbols - La cadena de símbolos que forman el alfabeto.
 * @throw VoidAlphabet Si la cadena de símbolos está vacía.
 * @throw EmptySymbol Si la cadena de símbolos contiene el símbolo vacío '&'.
 */
Alphabet::Alphabet(std::string symbols) {
  if (symbols.size() == 0) {
    throw VoidAlphabet("El alfabeto no puede ser vacío.");
  }
  if (symbols.find('&') != std::string::npos) {
    throw EmptySymbol("El símbolo '&' no es válido en un alfabeto, pues representa la cadena vacía.");
  }
  for (const char& ch : symbols) {
    Symbol symbol(ch);
    InsertSymbol(symbol);
  }
}

/**
 * @brief Función que dice si el alfabeto contiene el símbolo vacío '&'.
 * @return true si el alfabeto contiene el símbolo vacío '&', false en caso contrario.
 */
void Alphabet::InsertSymbol(const Symbol& symbol) {
  alphabet_.insert(symbol);
}

void Alphabet::RemoveSymbol(const Symbol& symbol) {
  if (alphabet_.find(symbol) == alphabet_.end()) {
    std::cerr << "Warning: El símbolo '" << symbol << "' no existe en el alfabeto." << std::endl;
    return;
  }
  alphabet_.erase(symbol);
}

/**
 * @brief Sobrecarga del operador de salida para la clase Alphabet.
 * @param os - flujo de salida
 * @param alphabet - alfabeto a imprimir
 * @return flujo de salida
 */
std::ostream& operator<<(std::ostream& os, const Alphabet& alphabet) {
  os << "{";
  bool first = true;
  for (const auto& symbol : alphabet.GetAlphabet()) {
    if (!first) os << ", ";
    os << symbol;
    first = false;
  }
  os << "}";
  return os;
}

/**
 * @brief Sobrecarga del operador de entrada para la clase Alphabet.
 * @param is - flujo de entrada
 * @param alphabet - alfabeto a leer
 * @return flujo de entrada
 */
std::istream& operator>>(std::istream& is, Alphabet& alphabet) {
  std::string symbols;
  is >> symbols;
  alphabet = Alphabet(symbols);
  return is;
}