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
 * @file main_functions.h
 * @brief Fichero que contiene las funciones auxiliares del programa.
 *
 * Historial de versiones
 *  15/10/2025 - Creación (primera versión) del código
 */

#ifndef MAIN_FUNCTIONS_H_
#define MAIN_FUNCTIONS_H_

#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

#include "automaton.h"

// Estructura para almacenar los argumentos del programa
struct Arguments {
  const char* input_file;
  const char* input_automaton_file;
};

// =============================================================================
// Funciones auxiliares
// =============================================================================

void CheckInputAutomatonFile(const char* filename, Arguments* args);
void CheckInputFile(const char* filename, Arguments* args);
void CheckArguments(int argc, char* argv[], Arguments* args);
void Usage(int argc, char* argv[]);
void AutomataInformation(const Automaton& automaton);

// =============================================================================
// Funciones para leer el autómata desde un archivo
// =============================================================================
Automaton ReadAutomaton(const std::string& filename);
void ProcessInputFile(const std::string& filename, const Automaton& automaton);
Alphabet ReadAlphabet(std::string& line, std::ifstream& file_in,
                      int line_number);
int ReadNumberOfStates(std::string& line, std::ifstream& file_in,
                       int line_number);
State ReadInitialState(std::string& line, std::ifstream& file_in,
                       int line_number);
void ReadStatesAndTransitions(const int& num_states, const Alphabet& alphabet,
                              std::ifstream& file_in,
                              std::set<State>& all_states,
                              Transition& all_transitions,
                              std::set<State>& final_states, int& line_number);
std::tuple<std::string, int, int> ReadStateInfo(std::ifstream& file_in,
                                                int line_number,
                                                int state_index);
void ReadTransitions(std::ifstream& file_in, const Alphabet& alphabet,
                     const State& state, int num_transitions,
                     Transition& all_transitions,
                     int& line_number);
void ValidateDestinationStates(const Transition& all_transitions,
                               const std::set<State>& state_names);
Automaton CreateAutomaton(const Alphabet& alphabet, const std::set<State>& all_states,
    const State& initial_state, const std::set<State>& final_states,
    const Transition& all_transitions);
bool IsDFA(const Automaton& automaton);

// =============================================================================
// Funciones de validación del autómata
// =============================================================================
void ValidateStateCount(int num_states, const Automaton& automaton);
void ValidateAutomaton(const Automaton& automaton);
void CheckInitialStateExists(const Automaton& automaton);
void CheckFinalStatesExist(const Automaton& automaton);
std::set<State> IsReachableStates(const Automaton& automaton);
void CheckFinalStatesReachable(const Automaton& automaton,
                               const std::set<State>& reachable);
void WarnUnreachableStates(const Automaton& automaton,
                           const std::set<State>& reachable);
void WarnEquivalentStates(const Automaton& automaton,
                        const Transition& equivalent_states);

#endif  // MAIN_FUNCTIONS_H_
