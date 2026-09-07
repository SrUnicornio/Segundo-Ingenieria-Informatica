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
 * @file chain.h
 * @brief Definición de la clase Chain
 *
 * Historial de versiones
 *   17/09/2025 - Creación y documentación
 *   15/10/2025 - Actualización de la documentación y eliminación de metodos innecesarios
 */

#ifndef CHAIN_H_
#define CHAIN_H_

#include <iostream>
#include <string>

#include "alphabet.h"
#include "errors.h"

const std::string kVoidChainSymbol = "&";

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
