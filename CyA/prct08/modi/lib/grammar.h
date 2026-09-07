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

#include <fstream>
#include <map>
#include <algorithm>
#include <sstream>
#include <vector>

#include "alphabet.h"

using ProductionSet = std::map<Symbol, std::vector<std::vector<Symbol>>>;

const std::string kProductionArrow = " \u2192 ";  // Flecha de producción
const Symbol kStartSymbol('S');                   // Símbolo inicial por defecto

/**
 * @class Grammar
 * @brief Representa una gramática independiente del contexto y permite su
 * conversión a Forma Normal de Chomsky.
 */
class Grammar {
 public:
  Grammar() : start_symbol_(kStartSymbol) {}

  // Getters para los símbolos y producciones
  unsigned int GetNumberOfTerminalSymbols() const {
    return terminal_symbols_.Size();
  }

  unsigned int GetNumberOfNonTerminalSymbols() const {
    return non_terminal_symbols_.Size();
  }

  unsigned int GetNumberOfProductions() const {
    unsigned int count = 0;
    for (const auto& [lhs, rhs_list] : production_rules_) {
      count += rhs_list.size();
    }
    return count;
  }

  // Métodos principales
  void LoadGrammar(const std::string& input_file_name);
  void ConvertToChomskyNormalForm();
  void SaveGrammar(const std::string& output_file_name) const;

  // Sobrecarga del operador de salida para imprimir la gramática
  friend std::ostream& operator<<(std::ostream& os, const Grammar& grammar);

 private:
  Alphabet terminal_symbols_;           /// Σ - Alfabeto de terminales
  Alphabet non_terminal_symbols_;       /// V - Conjunto de no terminales
  Symbol start_symbol_ = kStartSymbol;  /// S - Símbolo inicial
  ProductionSet production_rules_;      /// P - Reglas de producción

  // Métodos auxiliares
  bool IsStartSymbolDefined() const;
  void ReadSymbols(std::ifstream& input, int& line_number, Alphabet& alphabet,
                   bool is_terminal);
  void ReadProductions(std::ifstream& input, int& line_number);
  void ValidateLeftSide(const Symbol& left_side, int line_number);
  void ValidateSymbol(const Symbol& symbol, int line_number, bool is_terminal,
                      Alphabet& alphabet);
  void ValidateProduction(const Symbol& left_side,
                          const std::string& left_side_str,
                          const std::string& right_side_str, int line_number);
  void ValidateRightSide(const std::string& left_side_str,
                         const std::string& right_side_str, int line_number);
  void VerifyNoUselessProductions() const;
  std::set<Symbol> FindProductiveNonTerminals() const;
  std::set<Symbol> FindReachableNonTerminals() const;
  void ReplaceTerminalsInLongProductions();
  void BinarizeLongProductions();

  Symbol GenerateNewNonTerminal();

  // Modi elim producniones unitarias
  void RemoveUnitProductions();
};

#endif  // GRAMMAR_H_
