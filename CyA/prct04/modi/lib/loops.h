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
 * @file loops.h
 * @brief Fichero que contiene las funciones para el análisis de bucles
 *
 * Historial de versiones
 *   03/10/2025
 *     - Creación (primera versión) del código
 *     - Añadida la clase Loop para representar bucles en el código
 *     - Añadida la función estática Match para detectar declaraciones de bucles
 *     - Sobrecarga del operador de salida para la clase Loop
 */

#ifndef LOOPS_H_
#define LOOPS_H_

#include <iostream>
#include <string>

enum class LoopType { FOR, WHILE, DO_WHILE };

class Loop {
 public:
  // Constructor por defecto
  Loop() = default;
  Loop(LoopType type, int line) : type_(type), line_(line) {}
  ~Loop() = default;

  LoopType GetType() const { return type_; }
  int GetLine() const { return line_; }

  static bool Match(const std::string& line, Loop& result, int line_number);

  friend std::ostream& operator<<(std::ostream& os, const Loop& loop);

 private:
  LoopType type_ = LoopType::FOR;
  int line_ = 0;
};

#endif  // LOOPS_H_