/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 4: Code analyzer
 * @author 
 * @date 03/10/2025
 * @file main_functions.cc
 * @brief Fichero que contiene las funciones auxiliares del programa.
 *
 * Historial de versiones
 *   03/10/2025
 *     - Creación (primera versión) del código
 */

#include <fstream>
#include <iostream>
#include <regex>
#include <string>

#include "../lib/funcions_main.h"

/**
 * @brief Función que muestra el uso del programa
 * @param argc Número de argumentos
 * @param argv Vector de argumentos
 */
void Usage(int argc, char* argv[]) {
  if (argc != 3) {
    if (argc == 2) {
      std::string parameter{argv[1]};
      if (parameter == "--help" || parameter == "-h") {
        std::cout
            << "Description:\n"
            << "  This program analyzes a C++ source code file and extracts:\n"
            << "   - Declared variables (int and double)\n"
            << "   - Loops (for and while)\n"
            << "   - Existence of a main function\n"
            << "   - Comments (single-line // and multi-line /* */)\n\n";
        exit(EXIT_SUCCESS);
      }
    }
    std::cerr << "Uso: " << argv[0] << " input_file.cc output_file.txt\n"
              << "Pruebe " << argv[0] << " --help para más información."
              << "  - El input_file debe ser un fichero .cc, .h, .cpp o .hpp\n"
              << "  - El output_file debe ser un fichero .txt\n"
              << std::endl;
    exit(EXIT_FAILURE);
  }
}

/**
 * @brief Función que comprueba los argumentos del programa
 * @param argc Número de argumentos
 * @param argv Vector de argumentos
 * @param args Estructura para almacenar los argumentos
 */
void CheckArguments(int argc, char* argv[], Arguments* args) {
  Usage(argc, argv);
  CheckInputFile(argv[1], args);
  CheckOutputFile(argv[2], args);
}

/**
 * @brief Función que comprueba el fichero de entrada
 * @param filename Nombre del fichero de entrada
 * @param args Estructura para almacenar los argumentos
 */
void CheckInputFile(const char* filename, Arguments* args) {
  std::ifstream file_in{filename};
  if (!file_in) {
    std::cerr << "Error: No se pudo abrir el fichero de entrada " << filename
              << std::endl;
    exit(EXIT_FAILURE);
  }
  std::string file_extension =
      std::string(filename).substr(std::string(filename).find_last_of("."));
  if (file_extension != ".cc" && file_extension != ".h" &&
      file_extension != ".cpp" && file_extension != ".hpp") {
    std::cerr << "Error: El fichero de entrada debe ser un fichero .cc, .h, "
                 ".cpp o .hpp"
              << std::endl;
    exit(EXIT_FAILURE);
  }
  args->input_file = filename;
  file_in.close();
}

/**
 * @brief Función que comprueba el fichero de salida
 * @param filename Nombre del fichero de salida
 * @param args Estructura para almacenar los argumentos
 */
void CheckOutputFile(const char* filename, Arguments* args) {
  std::ofstream file_out{filename};
  if (!file_out) {
    std::cerr << "Error: No se pudo abrir el fichero de salida " << filename
              << std::endl;
    exit(EXIT_FAILURE);
  }
  std::string file_extension =
      std::string(filename).substr(std::string(filename).find_last_of("."));
  if (file_extension != ".txt") {
    std::cerr << "Error: El fichero de salida debe ser un fichero .txt"
              << std::endl;
    exit(EXIT_FAILURE);
  }
  args->output_file = filename;
  file_out.close();
}

/**
 * @brief Función que analiza las variables en una línea de código
 * @param line Línea de código a analizar
 * @param structure Estructura para almacenar el análisis del código
 * @param line_number Número de línea del código
 */
void AnalyzeVariables(const std::string& line, CodeStructure& structure,
                      int line_number) {
  // Try matching multiple declarations in the full line first
  std::vector<Variable> vars_multi;
  if (Variable::MatchMultiple(line, vars_multi, line_number)) {
    for (auto& v : vars_multi) structure.AddVariable(v);
  } else {
    // Fallback to single-variable match for backwards compatibility
    Variable var;
    if (Variable::Match(line, var, line_number)) structure.AddVariable(var);
  }
}

/**
 * @brief Función que analiza los bucles en una línea de código
 * @param line Línea de código a analizar
 * @param structure Estructura para almacenar el análisis del código
 * @param line_number Número de línea del código
 */
void AnalyzeLoops(const std::string& line, CodeStructure& structure,
                  int line_number) {
  Loop loop;
  if (Loop::Match(line, loop, line_number)) structure.AddLoop(loop);
}

/**
 * @brief Función que analiza los condicionales en una línea de código
 * @param line Línea de código a analizar
 * @param structure Estructura para almacenar el análisis del código
 * @param line_number Número de línea del código
 */
void AnalyzeConditionals(const std::string& line, CodeStructure& structure,
                         int line_number) {
  Conditional cond;
  if (Conditional::Match(line, cond, line_number))
    structure.AddConditional(cond);
}

/**
 * @brief Función que analiza los comentarios en una línea de código
 * @param line Línea de código a analizar
 * @param structure Estructura para almacenar el análisis del código
 * @param line_number Número de línea del código
 * @param inside_multiline Booleano que indica si se está dentro de un
 * comentario multilínea
 * @param multi_content Contenido acumulado del comentario multilínea
 * @param multi_start_line Línea de inicio del comentario multilínea
 */
void AnalyzeComments(const std::string& line, CodeStructure& structure,
                     int line_number, bool& inside_multiline,
                     std::string& multi_content, int& multi_start_line) {
  Comment comment;
  if (Comment::Match(line, comment, line_number, inside_multiline,
                     multi_content, multi_start_line)) {
    structure.AddComment(comment);
  }
}

/**
 * @brief Función que detecta la existencia de la función main en una línea de
 * código
 * @param line Línea de código a analizar
 * @param structure Estructura para almacenar el análisis del código
 */
void DetectMain(const std::string& line, CodeStructure& structure) {
  static const std::regex main_regex(R"(\bint\s+main\s*\(.*\))");
  if (std::regex_search(line, main_regex)) {
    structure.SetHasMain(true);
  }
}

/**
 * @brief Función que analiza el fichero de código fuente
 * @param filename Nombre del fichero de código fuente
 * @param structure Estructura para almacenar el análisis del código
 */
void AnalyzeFile(const std::string& filename, CodeStructure& structure) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Error: cannot open " << filename << "\n";
    return;
  }

  std::string line;
  int line_number = 0;
  bool inside_multiline = false;
  std::string multi_content;
  int multi_start_line = 0;

  while (getline(file, line)) {
    line_number++;
    AnalyzeVariables(line, structure, line_number);
    AnalyzeLoops(line, structure, line_number);
    AnalyzeConditionals(line, structure, line_number);
    AnalyzeComments(line, structure, line_number, inside_multiline,
                    multi_content, multi_start_line);
    DetectMain(line, structure);
  }
}

/**
 * @brief Función que escribe el análisis en el fichero de salida
 * @param filename - Nombre del fichero de salida
 * @param structure - Estructura que contiene el análisis del código
 */
void WriteAnalysisToFile(const char* filename, const CodeStructure& structure) {
  std::ofstream file_out{filename};
  if (!file_out) {
    std::cerr << "Error: No se pudo abrir el fichero de salida " << filename
              << std::endl;
    exit(EXIT_FAILURE);
  }
  file_out << structure;
  file_out.close();
}
