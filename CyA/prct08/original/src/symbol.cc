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
 * @file symbol.cc
 * @brief Fichero que contiene la definición de la clase Symbol
 * 
 * Historial de versiones
 *   17/09/2025
 *     - Creación (primera versión) del código
 *   15/10/2025
 *     - Actualización de la documentación
 */

#include "../lib/symbol.h"

/**
 * @brief Operador menor que para comparación de símbolos.
 * @param other - símbolo con el que comparar.
 * @return true si el símbolo actual es menor que el otro.
 */
bool Symbol::operator<(const Symbol& other) const {
  if (IsFNC_Symbol() || other.IsFNC_Symbol())
    return FNC_symbol_ < other.FNC_symbol_;
  return symbol_ < other.symbol_;
}

/**
 * @brief Operador de igualdad entre símbolos.
 * @param other - símbolo con el que comparar.
 * @return true si los símbolos son iguales.
 */
bool Symbol::operator==(const Symbol& other) const {
  if (IsFNC_Symbol() || other.IsFNC_Symbol())
    return FNC_symbol_ == other.FNC_symbol_;
  return symbol_ == other.symbol_;
}

/**
 * @brief Sobrecarga del operador de entrada y salida para la clase Symbol.
 * @param os - flujo de salida
 * @param symbol - símbolo a imprimir
 * @return flujo de salida
 */
std::ostream& operator<<(std::ostream& os, const Symbol& symbol) {
  if (symbol.IsFNC_Symbol())
    os << symbol.FNC_symbol_;
  else
    os << symbol.symbol_;
  return os;
}

/**
 * @brief Sobrecarga del operador de entrada para la clase Symbol.
 * @param is - flujo de entrada
 * @param symbol - símbolo a leer
 * @return flujo de entrada
 */
std::istream& operator>>(std::istream& is, Symbol& symbol) {
  std::string input;
  is >> input;
  if (input.size() == 1)
    symbol.symbol_ = input[0];
  else
    symbol.FNC_symbol_ = input;
  return is;
}