/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 8: Gramáticas en FNC
 *
 * @file Grammar2CNF.cc
 * @brief Programa principal que convierte una gramática a su forma normal de
 * Chomsky.
 */

#include "../lib/main_functions.h"

int main(int argc, char* argv[]) {
  Arguments args;
  try {
    CheckArguments(argc, argv, &args);

    Grammar grammar;
    Grammar copy_grammar;
    grammar.LoadGrammar(args.input_file);
    copy_grammar = grammar;
    grammar.ConvertToChomskyNormalForm();
    grammar.SaveGrammar(args.output_file);

    std::cout << "✅ Conversión completada correctamente.\n";
    ShowGrammarStatistics(copy_grammar);
    std::cout << "Gramática original:\n" << copy_grammar << "\n";
    ShowGrammarStatistics(grammar);
    std::cout << "Gramática en FNC:\n" << grammar << "\n";
  } catch (const Error& e) {
    std::cerr << "❌ Error: " << e.what() << "\n";
    return 1;
  } catch (...) {
    std::cerr << "❌ Error desconocido.\n";
    return 1;
  }

  return 0;
}
