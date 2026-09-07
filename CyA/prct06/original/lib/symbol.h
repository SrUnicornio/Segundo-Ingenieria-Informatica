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
 * @file symbol.h
 * @brief Definición de la clase Symbol
 *
 * Historial de versiones
 *   17/09/2025 - Creación y documentación
 *   15/10/2025 - Actualización de la documentación
 */

#ifndef SYMBOL_H_
#define SYMBOL_H_

#include <iostream>

/**
 * @class Symbol
 * @brief Representa un símbolo de un alfabeto.
 */
class Symbol {
 public:
  /**
   * @brief Constructor que inicializa el símbolo.
   * @param symbol - carácter del símbolo.
   */
  explicit Symbol(char symbol) : symbol_(symbol) {}

  /** @brief Destructor por defecto. */
  ~Symbol() = default;

  /** @brief Obtiene el carácter del símbolo. */
  char GetSymbol() const { return symbol_; }

  /**
   * @brief Modifica el carácter del símbolo.
   * @param symbol - nuevo carácter.
   * @return carácter modificado.
   */
  char SetSymbol(char symbol) { return symbol_ = symbol; }
  
  /** @brief Comprueba si el símbolo es el símbolo vacío '&'. */
  bool IsVoidChainSymbol() const { return symbol_ == '&'; }
  /**
   * @brief Operador menor que para comparación de símbolos.
   * @param other - símbolo con el que comparar.
   * @return true si el símbolo actual es menor que el otro.
   */
  bool operator<(const Symbol& other) const { return symbol_ < other.symbol_; }

  /**
   * @brief Operador de igualdad entre símbolos.
   * @param other - símbolo con el que comparar.
   * @return true si los símbolos son iguales.
   */
  bool operator==(const Symbol& other) const { return symbol_ == other.symbol_; }

  /**
   * @brief Operador de desigualdad entre símbolos.
   * @param other - símbolo con el que comparar.
   * @return true si los símbolos son distintos.
   */
  bool operator!=(const Symbol& other) const { return !(symbol_ == other.symbol_); }

  /** @brief Sobrecarga del operador de salida. */
  friend std::ostream& operator<<(std::ostream& os, const Symbol& symbol);

  /** @brief Sobrecarga del operador de entrada. */
  friend std::istream& operator>>(std::istream& is, Symbol& symbol);

 private:
  char symbol_;  /** Carácter que representa el símbolo. */

};

#endif  // SYMBOL_H_