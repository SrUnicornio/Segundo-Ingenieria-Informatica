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
 * @file transition.h
 * @brief Definición de la clase Transition
 * Historial de versiones
 *   15/10/2025 - Creación y documentación
 */

#ifndef TRANSITION_FUNCTION_H_
#define TRANSITION_FUNCTION_H_

#include <map>
#include <set>
#include <string>

#include "state.h"
#include "symbol.h"

const Symbol kVoidSymbol('&');        // Símbolo que representa la cadena vacía
const std::string kDelta = "\u03B4";  // Delta

/**
 * @class Transition
 * @brief Representa el conjunto de transiciones (δ) de un autómata finito.
 */
class Transition {
 public:
  Transition() = default;
  ~Transition() { transitions_.clear(); }

  const std::multimap<std::pair<State, Symbol>, std::set<State>>&
  GetTransitions() const {
    return transitions_;
  }
  void SetTransitions(const std::multimap<std::pair<State, Symbol>,
                                          std::set<State>>& transitions) {
    transitions_ = transitions;
  }

  void Insert(const Transition& transition);
  void Insert(const std::pair<State, Symbol>& key, const State& to_state);
  std::set<State> GetNextStates(const State& from, const Symbol& symbol) const;
  Transition operator[](const State& state) const;

  friend std::ostream& operator<<(std::ostream& os, const Transition& delta);

 private:
  std::multimap<std::pair<State, Symbol>, std::set<State>> transitions_;
};

#endif  // TRANSITION_FUNCTION_H_
