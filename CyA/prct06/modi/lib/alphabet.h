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
 * @file alphabet.h
 * @brief Definición de la clase Alphabet
 *
 * Historial de versiones
 *   17/09/2025 - Creación y documentación
 *   15/10/2025 - Actualización de la documentación
 */

#ifndef ALPHABET_H_
#define ALPHABET_H_

#include <iostream>
#include <set>
#include <string>

#include "errors.h"
#include "symbol.h"

/**
 * @class Alphabet
 * @brief Representa un alfabeto de símbolos distintos.
 */
class Alphabet {
 public:
  /** @brief Constructor que crea un alfabeto a partir de una cadena de símbolos. */
  explicit Alphabet(std::string symbols);

  /** @brief Destructor que limpia el alfabeto. */
  ~Alphabet() { alphabet_.clear(); }

  /**
   * @brief Devuelve el alfabeto como un conjunto de símbolos.
   * @return Conjunto de símbolos.
   */
  const std::set<Symbol>& GetAlphabet() const { return alphabet_; }

  /**
   * @brief Devuelve el número de símbolos en el alfabeto.
   * @return Tamaño del alfabeto.
   */
  int Size() const { return alphabet_.size(); }

  /** @brief Inserta un símbolo en el alfabeto. */
  void InsertSymbol(const Symbol& symbol);

  /** @brief Elimina un símbolo del alfabeto. */
  void RemoveSymbol(const Symbol& symbol);

  /**
   * @brief Comprueba si un símbolo pertenece al alfabeto.
   * @param symbol - Símbolo a comprobar.
   * @return true si el símbolo está en el alfabeto, false en caso contrario.
   */
  bool Contains(const Symbol& symbol) const {
    return alphabet_.count(symbol) > 0;
  }

  bool operator==(const Alphabet& other) const { return alphabet_ == other.alphabet_; }
  bool operator!=(const Alphabet& other) const { return !(*this == other); }
  
  /**
   * @brief Sobrecarga del operador de salida.
   */
  friend std::ostream& operator<<(std::ostream& os, const Alphabet& alphabet);

  /**
   * @brief Sobrecarga del operador de entrada.
   */
  friend std::istream& operator>>(std::istream& is, Alphabet& alphabet);

 private:
  void ValidateAlphabetSymbols(const std::string& symbol) const;

  std::set<Symbol> alphabet_; /**< Conjunto de símbolos únicos del alfabeto. */
};

#endif  // ALPHABET_H_
