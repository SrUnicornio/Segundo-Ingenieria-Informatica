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
 * @file main.cc
 * @brief Fichero que contiene la función main del programa.
 * 
 * Historial de versiones
 *   15/10/2025
 *     - Creación (primera versión) del código
 */

#include "../lib/functions_main.h"
#include <fstream>

int main(int argc, char* argv[]) {
  Arguments args;
  try {
    CheckArguments(argc, argv, &args);

    Automaton automaton =
        ReadAutomaton(std::string{args.input_automaton_file});
    ValidateAutomaton(automaton);
    if (IsDFA(automaton)) {
      std::cout << "El autómata es un DFA." << std::endl;
      WarnEquivalentStates(automaton, automaton.GetTransition());
    } else {
      std::cout << "El autómata es un NFA." << std::endl;
    }
    AutomataInformation(automaton);

    ProcessInputFile(args.input_file, automaton);
  } catch (const Error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  } catch (const std::exception& e) {
    std::cerr << "Error inesperado: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}