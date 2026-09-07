/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 4: Code analyzer
 * @author Álvaro Pérez Ramos - alu0101574042@ull.edu.es
 * @date 03/10/2025
 * @file code_structure.h
 * @brief Fichero que contiene las funciones para el análisis de la estructura
 * del código
 *
 * Historial de versiones
 *   03/10/2025
 *     - Creación (primera versión) del código
 *     - Añadida la clase CodeStructure para representar la estructura del
 * código
 *     - Añadida la sobrecarga del operador de salida para la clase
 * CodeStructure
 */

#ifndef CODE_STRUCTURE_H_
#define CODE_STRUCTURE_H_

#include <vector>

#include "comment.h"
#include "conditionals.h"
#include "directivas.h"
#include "loops.h"
#include "variable.h"

class CodeStructure {
 public:
  CodeStructure() = default;
  explicit CodeStructure(const std::string& input_file)
      : input_file_(input_file) {}
  ~CodeStructure() = default;

  const std::string& GetInputFile() const { return input_file_; }

  void AddComment(const Comment& comment) { comments_.push_back(comment); }
  const std::vector<Comment>& GetComments() const { return comments_; }

  void AddVariable(const Variable& variable) { variables_.push_back(variable); }
  const std::vector<Variable>& GetVariables() const { return variables_; }

  void AddLoop(const Loop& loop) { loops_.push_back(loop); }
  const std::vector<Loop>& GetLoops() const { return loops_; }

  void AddConditional(const Conditional& conditional) {
    conditionals_.push_back(conditional);
  }
  const std::vector<Conditional>& GetConditionals() const {
    return conditionals_;
  }

  void AddDirectiva(const Directivas& d) { directivas_.push_back(d); }
  const std::vector<Directivas>& GetDirectivas() const { return directivas_; }

  // Main
  void SetHasMain(bool exists) { has_main_ = exists; }
  bool HasMain() const { return has_main_; }

  friend std::ostream& operator<<(std::ostream& os,
                                  const CodeStructure& structure);

 private:
  std::string input_file_ = "";
  std::vector<Comment> comments_;
  std::vector<Variable> variables_;
  std::vector<Loop> loops_;
  std::vector<Conditional> conditionals_;
  std::vector<Directivas> directivas_;
  bool has_main_ = false;
};

#endif  // CODE_STRUCTURE_H_