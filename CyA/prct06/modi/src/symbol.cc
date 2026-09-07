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
 * @brief Sobrecarga del operador de entrada y salida para la clase Symbol.
 * @param os - flujo de salida
 * @param symbol - símbolo a imprimir
 * @return flujo de salida
 */
std::ostream& operator<<(std::ostream& os, const Symbol& symbol) {
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
  char input_char;
  is >> input_char;
  symbol.symbol_ = input_char;
  return is;
}