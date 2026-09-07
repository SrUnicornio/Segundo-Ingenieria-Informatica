/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 2: Cadenas y lenguajes
 * @author 
 * @date 17/09/2025
 * @file main_functions.cc
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

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../lib/main_functions.h"

const std::string kUsage = R"(Uso: ./p08_grammar2CNF input.gra output.gra
                                   Parámetros:
                                    input.gra   Gramática de entrasda
                                    output.gra  Gramática en FNC\n)
                              Prueba --help o -h para más información.)";

const std::string kHelpText = R"(Uso: ./p08_grammar2CNF input.gra output.gra

Simula autómatas finitos (DFA/NFA).

Parámetros:
  input.gra   Gramática de entrasda
  output.gra  Gramática en FNC

Formato .gra: 
  - La primera línea contiene el número de símbolos terminales.
  - Las siguientes líneas contienen los símbolos terminales, uno por línea.
  - A continuación, una línea con el número de símbolos no terminales.
  - Las siguientes líneas contienen los símbolos no terminales, uno por línea.
  - Luego, una línea con el número de producciones.
  - Las siguientes líneas contienen las producciones en el formato:
      A \u03B1
    donde A es el símbolo no terminal del lado izquierdo y \u03B1 es la
    secuencia de símbolos del lado derecho (terminales y/o no terminales).
  La cadena vacía, ε se representa mediante el carácter &.

Ejemplo:
  $ ./p08_grammar2CNF input.gra output.gra
)";

/**
 * @brief Muestra el uso correcto del programa
 * @param argc - Número de argumentos
 * @param argv - Argumentos
 */
void Usage(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << kUsage;
    exit(EXIT_FAILURE);
  }
  if(argc == 2 && (std::string(argv[1]) == "--help" || 
     std::string(argv[1]) == "-h")) {
    std::cout << kHelpText;
    exit(EXIT_SUCCESS);
  }
}

/**
 * @brief Comprueba que el fichero de entrada existe y se puede abrir
 * @param filename - Nombre del fichero de entrada
 * @throw std::cerr si el fichero no se puede abrir
 */
void CheckOutputFile(const char* filename, Arguments* args) {
  std::string filename_str{filename};
  if (filename_str.substr(filename_str.find_last_of(".") + 1) != "gra") {
    throw FileError("Error: El archivo de entrada debe tener extensión .gra.");
  }
  std::ofstream out_in{filename};
  if (!out_in.is_open()) {
    std::cerr << "Error: No se pudo abrir el fichero de entrada \"" << filename
              << "\"." << std::endl;
    exit(EXIT_FAILURE);
  }
  out_in.close();
  args->output_file = filename;
}

/**
 * @brief Comprueba que el fichero de salida se puede crear o abrir
 * @param filename - Nombre del fichero de salida
 * @throw std::cerr si el fichero no se puede abrir
 */
void CheckInputFile(const char* filename, Arguments* args) {
  std::string filename_str{filename};
  if (filename_str.substr(filename_str.find_last_of(".") + 1) != "gra") {
    throw FileError("Error: El archivo de autómata debe tener extensión .gra.");
  }
  std::ifstream file_in{filename};
  if (!file_in.is_open()) {
    std::cerr << "Error: No se pudo abrir el fichero de autómata \"" << filename
              << "\"." << std::endl;
    exit(EXIT_FAILURE);
  }
  file_in.close();
  args->input_file = filename;
}

/**
 * @brief Comprueba que los argumentos son correctos y los asigna a la
 * estructura Arguments
 * @param argc - Número de argumentos
 * @param argv - Argumentos
 * @param args - Estructura donde se almacenan los argumentos
 * @throw std::cerr si algún argumento no es válido
 */
void CheckArguments(int argc, char* argv[], Arguments* args) {
  Usage(argc, argv);
  CheckInputFile(argv[1], args);
  CheckOutputFile(argv[2], args);
}

void ShowGrammarStatistics(const Grammar& grammar) {
  std::cout << "Estadísticas de la gramática:\n";
  std::cout << "  Número de símbolos terminales: "
            << grammar.GetNumberOfTerminalSymbols() << "\n";
  std::cout << "  Número de símbolos no terminales: "
            << grammar.GetNumberOfNonTerminalSymbols() << "\n";
  std::cout << "  Número de producciones: "
            << grammar.GetNumberOfProductions() << "\n";
}