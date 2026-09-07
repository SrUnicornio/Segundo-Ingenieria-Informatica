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

#include "grammar.h"

// Estructura para almacenar los argumentos del programa
struct Arguments {
  const char* input_file;
  const char* output_file;
};

// =============================================================================
// Funciones auxiliares
// =============================================================================

void CheckInputFile(const char* filename, Arguments* args);
void CheckOutputFile(const char* filename, Arguments* args);
void CheckArguments(int argc, char* argv[], Arguments* args);
void Usage(int argc, char* argv[]);

void ShowGrammarStatistics(const Grammar& grammar);

#endif  // MAIN_FUNCTIONS_H_