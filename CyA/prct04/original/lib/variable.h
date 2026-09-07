/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 4: Code analyzer
 * @author 
 * @date 03/10/2025
 * @file variable.h
 * @brief Fichero que contiene las funciones para el análisis de variables
 *
 * Historial de versiones
 *   03/10/2025
 *     - Creación (primera versión) del código
 *     - Añadida la clase Variable para representar variables en el código
 *     - Añadida la función estática Match para detectar declaraciones de
 * variables
 *     - Sobrecarga del operador de salida para la clase Variable
 */

#ifndef VARIABLE_H_
#define VARIABLE_H_

#include <iostream>
#include <string>
#include <vector>

enum class VariableType { INT, DOUBLE, UNKNOWN };

class Variable {
 public:
  // Constructor por defecto
  Variable() = default;
  Variable(VariableType type, const std::string& name, int line,
           bool initialized, const std::string& initial_value = "")
      : type_(type),
        name_(name),
        line_(line),
        initialized_(initialized),
        initial_value_(initial_value) {}
  ~Variable() = default;

  // Getters
  VariableType GetType() const { return type_; }
  std::string GetName() const { return name_; }
  int GetLine() const { return line_; }
  bool IsInitialized() const { return initialized_; }
  std::string GetInitialValue() const { return initial_value_; }

  static bool Match(const std::string& line, Variable& result, int line_number);
  static bool MatchMultiple(const std::string& line,
                            std::vector<Variable>& results, int line_number);

  friend std::ostream& operator<<(std::ostream& os, const Variable& variable);

 private:
  VariableType type_ = VariableType::INT;
  std::string name_ = "";
  int line_ = 0;
  bool initialized_ = false;
  std::string initial_value_ = "";
};

#endif  // VARIABLE_H_