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
 * @file gramatica.h
 * @brief Definición de la clase RegularGrammar
 * Historial de versiones:
 *   15/10/2025 - Creación y documentación inicial
 */

#ifndef GRAMMAR_H_
#define GRAMMAR_H_

#include <map>

#include "automaton.h"

class Grammar {
 public:
  Grammar(const Automaton& automaton) {
    ConvertFromAutomaton(automaton);
  }
  friend std::ostream& operator<<(std::ostream& os, const Grammar& grammar);

 private:
  std::set<Symbol> alphabet_;
  std::map<std::string, std::vector<std::string>> productions_;
  std::vector<std::string> variable_order_;
  std::string start_symbol_;
  std::map<std::string, std::string> state_to_variable_;

  /**
   * @brief Convierte un autómata en su gramática regular equivalente.
   */
  void ConvertFromAutomaton(const Automaton& automaton);
  /**
   * @brief Asigna nombres de variables (S, A, B, C...) a los estados.
   */
  void AssignVariables(const std::set<State>& states, const State& initial);
};

#endif  // GRAMMAR_H_