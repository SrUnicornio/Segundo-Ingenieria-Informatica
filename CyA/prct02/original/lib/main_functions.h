/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 2: Cadenas y lenguajes
 * 
 * @author
 * @date 17/09/2025
 * @file main_functions.h
 * @brief Fichero que contiene las funciones auxiliares del programa.
 *
 * Historial de versiones
 *   17/09/2025
 *     - Creación (primera versión) del código
 *     - Implementación de las funciones de comprobación de argumentos
 *     - Implementación de las opciones del programa
 *     - Uso de std::function para procesar entradas genéricas
 *     - Mejora en la gestión de errores try-catch
 */

#ifndef MAIN_FUNCTIONS_H_
#define MAIN_FUNCTIONS_H_

#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

#include "chain.h"
#include "language.h"

// Función tipo para procesar entradas genéricas
using InputProcessor = std::function<void(const Chain&, std::ostream&, int)>;

const std::string kSigma = "\u03A3"; // Sigma
const std::string kOmega = "\u03C9"; // Omega

// Estructura para almacenar los argumentos del programa
struct Arguments {
  const char* input_file;
  const char* output_file;
  int opcode;
};

// Estructura para almacenar las líneas del fichero de entrada
struct InputLine {
  std::string chain;
  std::string alphabet;
};

// =============================================================================
// Funciones auxiliares
// =============================================================================

void CheckInputFile(const char* filename, Arguments* args);
void CheckOutputFile(const char* filename, Arguments* args);
void CheckOpcode(const char* opcode_str, Arguments* args);
void CheckArguments(int argc, char* argv[], Arguments* args);
void Usage(int argc, char* argv[]);
std::string convertirASubindice(int numero);
std::vector<InputLine> ReadInputFile(const std::string& filename,
                                     std::string& output_file);
void ShowInputFile(const std::vector<InputLine>& entradas, std::ostream& out = std::cout);
void ProcessOptions(const std::vector<InputLine>& entradas,
                    const std::string& output_file, int opcode);

// =============================================================================
// Opciones del programa
// =============================================================================

void ProcessEntries(const std::vector<InputLine>& entradas,
                    const std::string& output_file, InputProcessor processor);
void AlphabetOption(const std::vector<InputLine>& entradas,
                    const std::string& output_file);
void LengthOption(const std::vector<InputLine>& entradas,
                  const std::string& output_file);
void InverseOption(const std::vector<InputLine>& entradas,
                   const std::string& output_file);
void PrefixOption(const std::vector<InputLine>& entradas,
                  const std::string& output_file);
void SuffixOption(const std::vector<InputLine>& entradas,
                  const std::string& output_file);
void SubstringOption(const std::vector<InputLine>& entradas,
                     const std::string& output_file);
void SubsequenceOption(const std::vector<InputLine>& entradas,
                       const std::string& output_file);
                       
#endif  // MAIN_FUNCTIONS_H_
