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
 * @file state.h
 * @brief Definición de la clase State
 * Historial de versiones
 *   15/10/2025 - Creación y documentación
 */

#ifndef STATE_H_
#define STATE_H_

#include <iostream>
#include <set>
#include <string>

#include "symbol.h"

/**
 * @class State
 * @brief Representa un estado de un autómata finito.
 */
class State {
 public:
  State() = default;
  explicit State(const std::string& name, bool is_initial = false,
                 bool is_final = false)
      : name_(name), is_initial_(is_initial), is_final_(is_final) {}

  const std::string& GetName() const { return name_; }
  bool IsInitial() const { return is_initial_; }
  bool IsFinal() const { return is_final_; }

  void SetInitial(bool value) { is_initial_ = value; }
  void SetFinal(bool value) { is_final_ = value; }

  /** Operador menor para poder guardar estados en std::set */
  bool operator<(const State& other) const {
    if (name_.length() == other.name_.length()) {
      return name_ < other.name_;  // si miden lo mismo, orden alfabético
    }
    return name_.length() < other.name_.length();  // si no, orden por longitud
  }

  bool operator==(const State& other) const { return name_ == other.name_; }

  friend std::ostream& operator<<(std::ostream& os, const State& state) {
    os << state.name_;
    if (state.is_initial_) os << " (Inicial)";
    if (state.is_final_) os << " (Final)";
    return os;
  }
  
 private:
  std::string name_;
  bool is_initial_ = false;
  bool is_final_ = false;
};

#endif  // STATE_H_
