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
 * @file chain.h
 * @brief Definición de la clase Chain
 *
 * Historial de versiones
 *   17/09/2025 - Creación y documentación
 */

#ifndef CHAIN_H_
#define CHAIN_H_

#include <iostream>
#include <string>

#include "alphabet.h"
#include "errors.h"

class Language;

const std::string kVoidChainSymbol = "&";  // Símbolo que representa la cadena vacía

/**
 * @class Chain
 * @brief Representa una cadena de símbolos válida según un alfabeto.
 */
class Chain {
 public:
  /** @brief Constructor de la cadena. */
  explicit Chain(std::string chain, const Alphabet& alphabet);

  /** @brief Destructor que limpia la cadena. */
  ~Chain() { chain_.clear(); }

  /** @brief Devuelve la cadena como std::string. */
  const std::string& GetChain() const { return chain_; }

  /** @brief Devuelve el alfabeto asociado a la cadena. */
  const Alphabet& GetAlphabet() const { return alphabet_; }

  /** @brief Devuelve la longitud de la cadena (0 si es '&'). */
  int Length() const;

  /** @brief Devuelve la cadena inversa. */
  Chain Inverse() const;
  Chain operator~() const { return Inverse(); }

  /** @brief Calcula los prefijos de una cadena. */
  Language Prefix() const;

  /** @brief Calcula el sufijo de una cadena. */
  Language Suffix() const;

  // =============================================================================
  // Posibles modificaciones 
  // =============================================================================

  /** @brief Calcula el prefijo común más largo con otra cadena.*/
  Chain CommonPrefix(const Chain& other) const;

  /** @brief Calcula el sufijo común más largo con otra cadena.*/
  Chain CommonSuffix(const Chain& other) const;

  /** @brief Calcula las subcadenas de una cadena. */
  Language Substring() const;

  /** @brief Calcula las subsecuencias de una cadena. */
  Language Subsequence() const;

  /** @brief Calcula la concatenación de cadenas. */
  Chain Concatenate(const Chain& other) const;
  Chain operator*(const Chain& other) const { return Concatenate(other); }

  /** @brief Calcula las repeticiones de la cadena. */
  Chain Repeat(int number_repetitions) const;
  Chain operator^(int number_repetitions) const { return Repeat(number_repetitions); }

  /** @brief Comprueba si una cadena es igual a otra. */
  bool IsEqual(const Chain& other) const;

  /** @brief Comprueba si la cadena actual es subsecuencia de otra. */
  bool IsSubsequence(const Chain& other) const;

  /** @brief Comprueba si la cadena actual es subcadena (substring) de otra. */
  bool IsSubstring(const Chain& other) const;

  Language Modification(size_t max_modifications) const;
  //==============================================================================
  // Fin de posibles modificaciones
  //==============================================================================

  /** @brief Sobrecarga del operador de comparación menor que. */
  bool operator<(const Chain& other) const;

  /** @brief Sobrecarga del operador de igualdad. */
  bool operator==(const Chain& other) const { return chain_ == other.chain_; }

  /** @brief Sobrecarga del operador de salida. */
  friend std::ostream& operator<<(std::ostream& os, const Chain& chain);

  /** @brief Sobrecarga del operador de entrada. */
  friend std::istream& operator>>(std::istream& is, Chain& chain);

 private:
  std::string chain_; /** Cadena de símbolos. */
  Alphabet alphabet_; /** Alfabeto asociado. */

  /** @brief Comprueba que todos los símbolos de la cadena estén en el alfabeto. */
  bool IsInAlphabet() const;

  /** @brief Comprueba si la cadena contiene símbolo vacío '&'. */
  bool HasEmptySymbol() const;
};

#endif  // CHAIN_H_
