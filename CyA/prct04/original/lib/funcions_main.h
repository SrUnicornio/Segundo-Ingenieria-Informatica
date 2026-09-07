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
 * @file funcions_main.h
 * @brief Fichero que contiene las funciones del main
 *
 * Historial de versiones
 *   03/10/2025
 *     - Creación (primera versión) del código
 */

#ifndef FUNCIONS_MAIN_H_
#define FUNCIONS_MAIN_H_

#include <fstream>
#include <string>

#include "code_structure.h"

// Estructura para almacenar los argumentos del programa
struct Arguments {
  const char* input_file;
  const char* output_file;
};

// =============================================================================
// Funciones auxiliares
// =============================================================================

void Usage(int argc, char* argv[]);
void CheckArguments(int argc, char* argv[], Arguments* args);
void CheckInputFile(const char* filename, Arguments* args);
void CheckOutputFile(const char* filename, Arguments* args);

void AnalyzeFile(const std::string& filename, CodeStructure& structure);

void AnalyzeVariables(const std::string& line, CodeStructure& structure,
                      int line_number);
void AnalyzeLoops(const std::string& line, CodeStructure& structure,
                  int line_number);
void AnalyzeConditionals(const std::string& line, CodeStructure& structure,
                         int line_number);
void AnalyzeComments(const std::string& line, CodeStructure& structure,
                     int line_number, bool& inside_multiline,
                     std::string& multi_content, int& multi_start_line);
void DetectMain(const std::string& line, CodeStructure& structure);
void WriteAnalysisToFile(const char* filename, const CodeStructure& structure);

#endif  // FUNCIONS_MAIN_H_