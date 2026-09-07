/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 4: Code analyzer
 * 
 * @author 
 * @date 03/10/2025
 * @file conditionals.h
 * @brief Fichero que contiene las funciones para el análisis de condicionales
 *
 * Historial de versiones
 *   03/10/2025
 *     - Creación (primera versión) del código
 *     - Añadida la clase Conditional para representar condicionales en el código
 *     - Añadida la función estática Match para detectar declaraciones de condicionales
 *     - Sobrecarga del operador de salida para la clase Conditional
 */
#ifndef CONDITIONALS_H_
#define CONDITIONALS_H_

#include <iostream>
#include <string>

enum class ConditionalType { IF, SWITCH, ELSE_IF, ELSE };

class Conditional {
 public:
  // Constructor por defecto
  Conditional() = default;
  Conditional(ConditionalType type, int line) : type_(type), line_(line) {}
  ~Conditional() = default;
  
  ConditionalType GetType() const { return type_; }
  int GetLine() const { return line_; }

  static bool Match(const std::string& line, Conditional& result,
                    int line_number);

  friend std::ostream& operator<<(std::ostream& os,
                                  const Conditional& conditional);

 private:
  ConditionalType type_ = ConditionalType::IF;
  int line_ = 0;
};

#endif // CONDITIONAL_H_