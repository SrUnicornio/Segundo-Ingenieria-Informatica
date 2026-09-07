/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 6: Autómatas Finitos
 *
 * @author 
 * @date 15/10/2025
 * @file alphabet.cc
 * @brief Fichero que contiene la declaración de la clase Alphabet
 * 
 * Historial de versiones
 *   17/09/2025
 *     - Creación (primera versión) del código 
 *     - Cambio de std::vector a std::set para evitar símbolos repetidos
 *     - Sobrecarga de operadores y control de errores
 *     - Añadidos métodos para insertar y eliminar símbolos
 *  15/10/2025
 *     - Actualización de la documentación
 */

#include "../lib/alphabet.h"

/**
 * @brief Constructor de la clase Alphabet.
 * @param symbols - La cadena de símbolos que forman el alfabeto.
 * @throw VoidAlphabet Si la cadena de símbolos está vacía.
 * @throw EmptySymbol Si la cadena de símbolos contiene el símbolo vacío '&'.
 */
Alphabet::Alphabet(std::string alphabet_symbols) {
  ValidateAlphabetSymbols(alphabet_symbols);
  for (const char& symbols : alphabet_symbols) {
    Symbol symbol(symbols);
    InsertSymbol(symbol);
  }
}

/**
 * @brief Valida un símbolo para asegurarse de que no es vacío ni el símbolo '&'.
 * @param symbol 
 */
void Alphabet::ValidateAlphabetSymbols(
    const std::string& alphabet_symbols) const {
  if (alphabet_symbols.size() == 0) {
    throw VoidAlphabet("El símbolo no puede ser vacío.");
  }
  if (alphabet_symbols == "&") {
    throw EmptySymbol(
        "El símbolo '&' no es válido en un alfabeto, pues representa la cadena "
        "vacía.");
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