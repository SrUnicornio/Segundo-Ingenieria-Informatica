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
 * @file language.h
 * @brief Fichero que contiene la definición de la clase Language
 */

#ifndef LANGUAGE_H_
#define LANGUAGE_H_

#include <iostream>
#include <set>

#include "chain.h"
#include "errors.h"

/**
 * @class Language
 * @brief Representa un lenguaje formal, es decir, un conjunto de cadenas sobre
 * un alfabeto.
 */
class Language {
 public:
  /**
   * @brief Constructor por defecto: lenguaje vacío.
   */
  Language() : language_set_(), alphabet_("a") {}

  /**
   * @brief Constructor a partir de una sola cadena.
   * @param chain Cadena inicial.
   */
  explicit Language(const Chain& chain) : alphabet_(chain.GetAlphabet()) {
    Insert(chain);
  }

  /** @brief Destructor por defecto. */
  ~Language() { Clear(); }

  /** @brief Limpia el lenguaje. */
  void Clear();

  /** @brief Inserta una cadena en el lenguaje. */
  void Insert(const Chain& chain) { language_set_.insert(chain); }

  /** @brief Elimina una cadena del lenguaje. */
  void Remove(const Chain& chain);

  /** @brief Devuelve el número de cadenas del lenguaje. */
  size_t Size() const { return language_set_.size(); }

  /** @brief Devuelve el conjunto de cadenas del lenguaje. */
  const std::set<Chain>& GetLanguageSet() const { return language_set_; }

  Language Concatenate(const Language& other) const;
  Language operator*(const Language& other) const;

  Language Power(size_t n) const;
  Language operator^(size_t n) const { return Power(n); }

  Language Union(const Language& other) const;

  Language Intersection(const Language& other) const;

  bool IsSubsetOf(const Language& other) const;

  bool operator==(const Language& other) const;

  Language Kleene(size_t max_power) const;

  Language Positive(size_t max_power) const;

  Language Difference(const Language& other) const;

  /**
   * @brief Sobrecarga del operador de salida para imprimir el lenguaje.
   */
  friend std::ostream& operator<<(std::ostream& os, const Language& language);

  /**
   * @brief Sobrecarga del operador de entrada para leer un lenguaje.
   * Formato esperado: número de cadenas seguido de cada cadena.
   */
  friend std::istream& operator>>(std::istream& is, Language& language);

 private:
  std::set<Chain> language_set_;
  Alphabet alphabet_; /** Alfabeto asociado. */
  
  bool Contains(const Chain& chain) const {
    return language_set_.find(chain) != language_set_.end();
  }
};

#endif  // LANGUAGE_H_
