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

#include "../lib/transition.h"

/**
 * @brief Inserta las transiciones de otro objeto Transition en el actual.
 * @param transition - objeto Transition cuyas transiciones se van a insertar.
 */
void Transition::Insert(const Transition& transition) {
  for (const auto& entry : transition.transitions_) {
    transitions_.insert(entry);
  }
}

/**
 * @brief Inserta una transición en la función de transición.
 * @param key - par que representa el estado de origen y el símbolo de entrada.
 * @param to_state - estado de destino al que se realiza la transición.
 */
void Transition::Insert(const std::pair<State, Symbol>& key,
                      const State& to_state) {
  auto range = transitions_.equal_range(key);
  if (range.first == range.second) {
    transitions_.insert(std::make_pair(key, std::set<State>{to_state}));
  } else {
    // Insert into the first matching entry's set
    range.first->second.insert(to_state);
  }
}

/**
 * @brief Obtiene los estados destino para una transición dada.
 * @param from - estado de origen.
 * @param symbol - símbolo de entrada.
 * @return conjunto de estados destino.
 */
std::set<State> Transition::GetNextStates(const State& from, const Symbol& symbol) const {
  std::set<State> next_states;
  auto range = transitions_.equal_range({from, symbol});
  for (auto it = range.first; it != range.second; ++it) {
    next_states.insert(it->second.begin(), it->second.end());
  }
  return next_states;
}

/**
 * @brief Operador de acceso para obtener las transiciones desde un estado dado.
 * @param state - estado de origen.
 * @return conjunto de transiciones desde el estado dado.
 */
Transition Transition::operator[](const State& state) const {
  Transition result;
  for (const auto& entry : transitions_) {
    if (entry.first.first == state) {
      result.transitions_.insert(entry);
    }
  }
  return result;
}

/**
 * @brief Sobrecarga del operador de salida para la clase Transition.
 * @param os - flujo de salida
 * @param delta - función de transición a imprimir
 * @return flujo de salida
 */
std::ostream& operator<<(std::ostream& os,
                                const Transition& delta) {
  os << "Función de transición (δ):\n";
  for (const auto& entry : delta.transitions_) {
    const State& from = entry.first.first;
    const Symbol& symbol = entry.first.second;
    const std::set<State>& to_states = entry.second;
    os << "  δ(" << from.GetName() << ", " << (symbol.GetSymbol() == '&' ? std::string("ε") : std::string(1, symbol.GetSymbol()))
        << ") -> {";
    for (const auto& to : to_states) {
      os << to.GetName();
      if (to.GetName() != to_states.rbegin()->GetName()) {
        os << ", ";
      }
    }
    os << "}\n";
  }
  return os;
}
