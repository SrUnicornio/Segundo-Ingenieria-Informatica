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
 *
 * Historial de versiones
 *   17/09/2025 - Creación y documentación
 *   15/10/2025 - Actualización de la documentación
 */
#include "../lib/automaton.h"

/**
 * @brief Comprueba si el Automaton acepta una cadena, considerando ε-transiciones.
 * @param chain - La cadena a comprobar.
 * @return true si la cadena es aceptada, false en caso contrario.
 */
bool Automaton::Accepts(const Chain& chain) const {
  std::set<State> current_states = EpsilonClosure({GetInitialState()});

  for (char ch : chain.GetChain()) {
    Symbol symbol(ch);
    std::set<State> next_states;

    // Para cada estado actual, obtener transiciones con el símbolo
    for (const auto& state : current_states) {
      std::set<State> reachable = GetTransition().GetNextStates(state, symbol);
      next_states.insert(reachable.begin(), reachable.end());
    }

    // Expandir con ε-transiciones
    current_states = EpsilonClosure(next_states);

    if (current_states.empty())
      return false;  // No hay estados a los que avanzar
  }

  // La cadena se acepta si algún estado actual es final
  for (const auto& state : current_states) {
    if (IsFinalState(state)) return true;
  }
  return false;
}

/**
 * @brief Calcula el ε-closure de un conjunto de estados
 * @param states - Conjunto de estados
 * @return Conjunto de estados alcanzables mediante ε-transiciones
 */
std::set<State> Automaton::EpsilonClosure(const std::set<State>& states) const {
  std::set<State> closure = states;
  std::queue<State> q;
  for (const auto& s : states) q.push(s);

  while (!q.empty()) {
    State s = q.front();
    q.pop();
    std::set<State> eps_next = GetTransition().GetNextStates(s, kVoidSymbol);
    for (const auto& next : eps_next) {
      if (closure.find(next) == closure.end()) {
        closure.insert(next);
        q.push(next);
      }
    }
  }

  return closure;
}

/**
 * @brief Sobrecarga del operador de salida para la clase Automaton.
 * @param os - flujo de salida
 * @param automaton - autómata a imprimir
 * @return flujo de salida
 */
std::ostream& operator<<(std::ostream& os, const Automaton& automaton) {
  os << kSigma << ": " << automaton.alphabet_ << "\n";
  os << "\nEstados:\n";
  for (const auto& state : automaton.states_) {
    os << "  " << state;
    if (state == automaton.initial_state_) {
      os << " (Inicial)";
    }
    if (automaton.final_states_.count(state) > 0) {
      os << " (Final)";
    }
    os << "\n";
  }
  
  os << "\n";
  os << automaton.delta_;
  return os;
}
