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
 * @file automaton.h
 * @brief Definición de la clase Automaton
 * Historial de versiones:
 *   15/10/2025 - Creación y documentación inicial
 */

#ifndef AUTOMATON_H_
#define AUTOMATON_H_

#include <iostream>
#include <queue>
#include <set>
#include <string>

#include "alphabet.h"
#include "chain.h"
#include "state.h"
#include "transition.h"

const std::string kSigma = "\u03A3";  // Sigma
const std::string kOmega = "\u03C9";  // Omega

/**
 * @class Automaton
 * @brief Representa un autómata finito (determinista o no determinista).
 *
 * Un automata es una T-Upla (Q, Σ, δ, s, F) donde:
 * - Σ es un alfabeto finito de símbolos.
 * - Q es un conjunto finito de estados.
 * - s ∈ Q es el estado inicial.
 * - F ⊆ Q es el conjunto de estados de aceptación (finales).
 * - δ: Q × Σ → P(Q) es la función de transición.
 */
class Automaton {
 public:
  /// Constructor por defecto.
  Automaton(const Alphabet& alphabet) : alphabet_(alphabet) {}
  virtual ~Automaton() {
    states_.clear();
    final_states_.clear();
  }

  const Alphabet& GetAlphabet() const { return alphabet_; }
  const std::set<State>& GetStates() const { return states_; }
  const State& GetInitialState() const { return initial_state_; }
  const std::set<State>& GetFinalStates() const { return final_states_; }
  const Transition& GetTransition() const { return delta_; }

  void InsertState(const std::set<State>& state) { states_ = state; }
  void SetAlphabet(const Alphabet& alphabet) { alphabet_ = alphabet; }
  void SetInitialState(const State& state) { initial_state_ = state; }
  void AddFinalState(const std::set<State>& states) { final_states_ = states; }
  void InsertTransition(const Transition& transition) {
    delta_.Insert(transition);
  }

  /**
   * @brief Comprueba si la NFA acepta una cadena, considerando ε-transiciones.
   */
  bool Accepts(const Chain& chain) const;
  /** @brief Calcula el ε-closure de un conjunto de estados */
  std::set<State> EpsilonClosure(const std::set<State>& states) const;

  /// Imprime la información completa del autómata.
  friend std::ostream& operator<<(std::ostream& os, const Automaton& automaton);

 private:
  Alphabet alphabet_;
  std::set<State> states_;
  std::set<State> final_states_;
  State initial_state_;
  Transition delta_;

  /**
   * @brief Comprueba si un estado es final
   */
  bool IsFinalState(const State& state) const {
    return final_states_.count(state) > 0;
  }
};

#endif  // AUTOMATON_H_
