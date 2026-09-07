/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 8: Gramáticas en FNC
 *
 * @author 
 * @date 29/11/2025
 * @file symbol.h
 * @brief Definición de la clase Symbol
 *
 * Historial de versiones
 *   17/09/2025 - Creación y documentación
 *   29/11/2025 - Actualización de la documentación
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
  Symbol() = default;
  /** @brief Constructor que inicializa el símbolo con un carácter. */
  explicit Symbol(const char& symbol) : symbol_(symbol) {}
  /** @brief Constructor que inicializa el símbolo con un identificador FNC. */
  explicit Symbol(const std::string& FNC_symbol)
      : symbol_(FNC_symbol.empty() ? '\0' : FNC_symbol[0]),
        FNC_symbol_(FNC_symbol) {}

  /** @brief Destructor por defecto. */
  ~Symbol() = default;

  /** @brief Obtiene la representación principal del símbolo. */
  char GetSymbol() const { return symbol_; }

  /** @brief Obtiene la forma extendida FNC (Ca, Cb, D1, etc.) */
  std::string GetFNC_Symbol() const { return FNC_symbol_; }

  /** @brief Modifica el carácter del símbolo. */
  char SetSymbol(char symbol) { return symbol_ = symbol; }

  /** @brief Modifica la forma FNC del símbolo. */
  std::string SetFNC_Symbol(const std::string& FNC_symbol) {
    FNC_symbol_ = FNC_symbol;
    if (!FNC_symbol_.empty()) symbol_ = FNC_symbol_[0];
    return FNC_symbol_;
  }

  /** @brief Comprueba si el símbolo es la cadena vacía. */
  bool IsVoidChainSymbol() const { return symbol_ == '&'; }

  /** @brief Comprueba si el símbolo es un símbolo FNC (introducido
   * artificialmente). */
  bool IsFNC_Symbol() const { return !FNC_symbol_.empty(); }

  /**
   * @brief Operador menor que (para sets y maps).
   * Compara por FNC_symbol_ si existe, o por symbol_ en su defecto.
   */
  bool operator<(const Symbol& other) const;

  /** @brief Operador de igualdad. */
  bool operator==(const Symbol& other) const;

  /** @brief Operador de desigualdad. */
  bool operator!=(const Symbol& other) const { return !(*this == other); }

  /** @brief Sobrecarga del operador de salida. */
  friend std::ostream& operator<<(std::ostream& os, const Symbol& symbol);

  /** @brief Sobrecarga del operador de entrada. */
  friend std::istream& operator>>(std::istream& is, Symbol& symbol);

 private:
  char symbol_; /** Carácter que representa el símbolo. */
  std::string FNC_symbol_; /** Nombre del símbolo en FNC (Ca, Cb, ...). */
};

#endif  // SYMBOL_H_