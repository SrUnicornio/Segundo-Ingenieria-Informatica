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

#include "../lib/grammar.h"

std::ostream& operator<<(std::ostream& os, const Grammar& grammar) {
  // os << "=== Gramática Regular equivalente ===\n";
  // os << "Símbolos terminales: ";
  for (const auto& s : grammar.alphabet_) os << s.GetSymbol() << " ";
  // os << "\nSímbolos no terminales: ";
  os << "\n" << grammar.variable_order_.size() << "\n";
  os << grammar.start_symbol_ << "\n";
  for (const auto& [lhs, rhs_list] : grammar.productions_) {
    os << lhs << " → ";
    for (size_t i = 0; i < rhs_list.size(); ++i) {
      os << rhs_list[i];
      if (i != rhs_list.size() - 1) os << " | ";
    }
    os << "\n";
  }
  return os;
}

void Grammar::ConvertFromAutomaton(const Automaton& automaton) {
  alphabet_ = automaton.GetAlphabet().GetAlphabet();
  const auto& states = automaton.GetStates();
  const auto& finals = automaton.GetFinalStates();
  const auto& delta = automaton.GetTransition();
  const auto& initial = automaton.GetInitialState();

  // Asignar letras a estados
  AssignVariables(states, initial);

  start_symbol_ = state_to_variable_[initial.GetName()];

  // Construir reglas de producción
  for (const auto& state : states) {
    std::string lhs = state_to_variable_[state.GetName()];
    std::vector<std::string> rhs;

    for (const auto& symbol : alphabet_) {
      std::set<State> next_states = delta.GetNextStates(state, symbol);
      for (const auto& next : next_states) {
        std::string dest_var = state_to_variable_[next.GetName()];
        rhs.push_back(std::string(1, symbol.GetSymbol()) + dest_var);

        // Si el estado destino es final → también producción directa
        if (finals.count(next)) {
          rhs.push_back(std::string(1, symbol.GetSymbol()));
        }
      }
    }

    // Si el estado mismo es final → producción ε
    if (finals.count(state)) rhs.push_back("&");

    if (!rhs.empty()) productions_[lhs] = rhs;
  }
}

/**
 * @brief Asigna nombres de variables (S, A, B, C...) a los estados.
 */
void Grammar::AssignVariables(const std::set<State>& states, const State& initial) {
  std::vector<std::string> available;
  for (char c = 'A'; c <= 'Z'; ++c) {
    if (c != 'S') available.push_back(std::string(1, c));
  }

  // Inicial siempre mapeado a S
  state_to_variable_[initial.GetName()] = "S";
  variable_order_.push_back("S");

  int idx = 0;
  for (const auto& st : states) {
    if (st == initial) continue;

    std::string symbol;
    if (idx < static_cast<int>(available.size())) {
      symbol = available[idx++];
    } else {
      // En caso de más de 26 estados
      symbol = available[idx % 25] + std::to_string(idx / 25);
      ++idx;
    }

    state_to_variable_[st.GetName()] = symbol;
    variable_order_.push_back(symbol);
  }
}
