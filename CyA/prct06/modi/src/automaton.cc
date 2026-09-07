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

#include <stack>

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

/**
 * @brief Traza el procesamiento de una cadena a través del autómata.
 * @param input - cadena de entrada a trazar
 */
void Automaton::TraceString(const std::string& input) const {
  std::cout << "\n\n=== Trazando cadena: '" << input << "' ===\n";

  struct Trace {
    std::set<State> current_states;
    std::string remaining;
    std::vector<std::string> path;
  };

  std::stack<Trace> stack;
  std::set<State> start_closure = EpsilonClosure({initial_state_});
  std::vector<std::string> init_path = {initial_state_.GetName()};
  stack.push({start_closure, input, init_path});

  bool accepted = false;
  std::set<std::string> printed_paths;

  while (!stack.empty()) {
    auto [current_set, remaining, path] = stack.top();
    stack.pop();

    // Si no queda entrada, comprueba si algún estado actual es final
    if (remaining.empty()) {
      for (const auto& state : current_set) {
        if (final_states_.count(state)) {
          accepted = true;
          std::string path_str;
          for (const auto& s : path) path_str += s + " -> ";
          path_str = path_str.substr(0, path_str.size() - 4);
          if (!printed_paths.count(path_str)) {
            std::cout << "✔ Aceptada: " << path_str
                      << " (Estado final: " << state.GetName() << ")\n";
            printed_paths.insert(path_str);
          }
        }
      }
      continue;
    }

    Symbol sym(remaining[0]);
    std::string next_input = remaining.substr(1);

    std::set<State> next_states;
    for (const auto& state : current_set) {
      auto reached = delta_.GetNextStates(state, sym);
      next_states.insert(reached.begin(), reached.end());
    }

    // Añadir cierres epsilon de los nuevos estados
    next_states = EpsilonClosure(next_states);

    if (next_states.empty()) continue;

    // Empuja el nuevo estado conjunto con el símbolo consumido
    for (const auto& next : next_states) {
      auto new_path = path;
      new_path.push_back(next.GetName());
      stack.push({{next}, next_input, new_path});
    }
  }

  if (!accepted) std::cout << "✘ Cadena rechazada por el autómata.\n";

  std::cout << "==============================\n\n";
}
