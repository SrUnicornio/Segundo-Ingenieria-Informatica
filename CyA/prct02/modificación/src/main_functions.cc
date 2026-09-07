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

#include "../lib/main_functions.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

/**
 * @brief Convierte un número entero a su representación en subíndices
 * @param numero - Número entero a convertir
 * @return Representación en subíndices del número
 */
std::string convertirASubindice(int numero) {
  static const std::string subindices[] = {"₀", "₁", "₂", "₃", "₄",
                                           "₅", "₆", "₇", "₈", "₉"};
  if (numero == 0) {
    return subindices[0];
  }
  std::string resultado;
  for (char digito : std::to_string(numero)) {
    if (digito >= '0' && digito <= '9') {
      resultado += subindices[digito - '0'];
    } else {
      resultado += '?';
    }
  }
  return resultado;
}

/**
 * @brief Muestra el uso correcto del programa
 * @param argc - Número de argumentos
 * @param argv - Argumentos
 */
void Usage(int argc, char* argv[]) {
  if (argc != 4) {
    if (argc == 2) {
      std::string parameter{argv[1]};
      if (parameter == "--help" || parameter == "-h") {
        std::cout << "Este programa recibe tres parámetros:\n"
                     "  1. Archivo de entrada: contiene cadenas separadas por "
                     "saltos de línea.\n"
                     "  2. Archivo de salida: donde se guardará la información "
                     "procesada.\n"
                     "  3. Opcode: indica la operación a realizar sobre las "
                     "cadenas.\n"
                     "\n"
                     "Opcodes disponibles:\n"
                     "  1) Alfabeto\n"
                     "  2) Longitud\n"
                     "  3) Inversas\n"
                     "  4) Prefijos\n"
                     "  5) Sufijos\n"
                     "  6) Modificación\n"
                     "\n"
                     "Ejemplo de formato del archivo de entrada:\n"
                     "  abbab ab\n"
                     "  6793836 123456789\n"
                     "  hola  ahlo\n"
                     "\n"
                     "El opcode determina qué información se almacenará en el "
                     "archivo de salida."
                  << std::endl;
        exit(EXIT_SUCCESS);
      }
    }
    std::cout << "Modo de empleo: " << argv[0]
              << " entrada.txt salida.txt opcode\n"
              << "Pruebe: " << argv[0] << " --help para más información."
              << std::endl;
    exit(EXIT_FAILURE);
  }
}

/**
 * @brief Comprueba que el fichero de entrada existe y se puede abrir
 * @param filename - Nombre del fichero de entrada
 * @throw std::cerr si el fichero no se puede abrir
 */
void CheckInputFile(const char* filename, Arguments* args) {
  std::string filename_str{filename};
  if (filename_str.substr(filename_str.find_last_of(".") + 1) != "txt") {
    throw FileError("Error: El archivo de entrada debe tener extensión .txt.");
  }
  std::ifstream file_in{filename};
  if (!file_in.is_open()) {
    std::cerr << "Error: No se pudo abrir el fichero de entrada \"" << filename
              << "\"." << std::endl;
    exit(EXIT_FAILURE);
  }
  file_in.close();
  args->input_file = filename;
}

/**
 * @brief Comprueba que el fichero de salida se puede crear o abrir
 * @param filename - Nombre del fichero de salida
 * @throw std::cerr si el fichero no se puede abrir
 */
void CheckOutputFile(const char* filename, Arguments* args) {
  std::string filename_str{filename};
  if (filename_str.substr(filename_str.find_last_of(".") + 1) != "txt") {
    throw FileError("Error: El archivo de salida debe tener extensión .txt.");
  }
  std::ofstream file_out{filename};
  if (!file_out.is_open()) {
    std::cerr << "Error: No se pudo abrir el fichero de salida \"" << filename
              << "\"." << std::endl;
    exit(EXIT_FAILURE);
  }
  file_out.close();
  args->output_file = filename;
}

/**
 * @brief Comprueba que el opcode es un número entero entre 1 y 7
 * @param opcode_str - Opcode en formato cadena
 * @throw std::cerr si el opcode no es válido
 */
void CheckOpcode(const char* opcode_str, Arguments* args) {
  std::string opcode{opcode_str};
  if (opcode.empty() || !std::all_of(opcode.begin(), opcode.end(), ::isdigit)) {
    std::cerr << "Error: El opcode debe ser un número entero." << std::endl;
    exit(EXIT_FAILURE);
  }
  int opcode_num = std::stoi(opcode);
  if (opcode_num < 1 || opcode_num > 7) {
    std::cerr << "Error: El opcode debe estar entre 1 y 7." << std::endl;
    exit(EXIT_FAILURE);
  }
  args->opcode = opcode_num;
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
  CheckOpcode(argv[3], args);
}

/**
 * @brief Lee el fichero de entrada y devuelve un vector de InputLine
 * @param filename - Nombre del fichero de entrada
 * @param output_file - Nombre del fichero de salida para reportar errores
 * @return Vector de InputLine con las cadenas y alfabetos leídos
 */
std::vector<InputLine> ReadInputFile(const std::string& filename,
                                     std::string& output_file) {
  std::ifstream file_in{filename};
  std::vector<InputLine> lines;
  std::string line;
  int line_count = 0;

  while (std::getline(file_in, line)) {
    line_count++;
    if (line.empty()) continue;
    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
      tokens.push_back(token);
    }
    if (tokens.size() == 2) {
      lines.push_back({tokens[0], tokens[1]});
    } else if (tokens.size() == 1) {
      lines.push_back({tokens[0], tokens[0]});
    } else {
      std::ofstream file_out{output_file, std::ios::app};
      file_out
          << "Error en la línea " << line_count
          << ": más de dos elementos encontrados."
          << "El formato correcto es: <cadena> <alfabeto> o solo <alfabeto>."
          << std::endl;
      exit(EXIT_FAILURE);
    }
  }
  return lines;
}

/**
 * @brief Muestra el contenido del fichero de entrada por pantalla
 * @param filename - Nombre del fichero de entrada
 * @param out - Flujo de salida (por defecto std::cout)
 */
void ShowInputFile(const std::vector<InputLine>& entradas, std::ostream& out) {
  int index = 1;
  for (const auto& entrada : entradas) {
    Chain chain(entrada.chain, Alphabet(entrada.alphabet));
    std::string subindex = convertirASubindice(index);
    out << kOmega << subindex << " = " << chain.GetChain() << std::endl;
    out << kSigma << subindex << " = " << chain.GetAlphabet() << std::endl;
    out << "Longitud de " << kOmega << subindex << " = " << chain.Length()
        << std::endl;
    out << "----------------------------------" << std::endl;
    index++;
  }
}

/**
 * @brief Procesa las entradas según la operación seleccionada y guarda los
 * resultados en el fichero de salida
 * @param entradas - Vector de entradas con cadenas y alfabetos
 * @param output_file - Nombre del fichero de salida
 * @param opcode - Operación a realizar (1-5)
 */
void ProcessOptions(const std::vector<InputLine>& entradas,
                    const std::string& output_file, int opcode) {
  switch (opcode) {
    case 1:
      AlphabetOption(entradas, output_file);
      break;
    case 2:
      LengthOption(entradas, output_file);
      break;
    case 3:
      InverseOption(entradas, output_file);
      break;
    case 4:
      PrefixOption(entradas, output_file);
      break;
    case 5:
      SuffixOption(entradas, output_file);
      break;
    case 6:
      ModificationOption(entradas, output_file);
      break;
    default:
      std::cerr << "No deberías estar aquí. Algo ha ido mal." << std::endl;
      exit(EXIT_FAILURE);
  }
}

/**
 * @brief Procesa las entradas según la operación seleccionada y guarda los
 * resultados en el fichero de salida
 * @param entradas - Vector de entradas con cadenas y alfabetos
 * @param output_file - Nombre del fichero de salida
 * @param processor - Función que procesa cada cadena y escribe el resultado en
 * el fichero de salida
 */
void ProcessEntries(const std::vector<InputLine>& entradas,
                    const std::string& output_file, InputProcessor processor) {
  std::ofstream file_out{output_file};
  int index = 1;
  for (const auto& entrada : entradas) {
    try {
      Alphabet alfabeto(entrada.alphabet);
      Chain cadena(entrada.chain, alfabeto);
      processor(cadena, file_out, index);
      index++;
    } catch (const std::exception& e) {
      file_out << e.what() << std::endl;
    }
  }
}

/**
 * @brief Opción para calcular y guardar los alfabetos de las cadenas
 * @param entradas - Vector de entradas con cadenas y alfabetos
 * @param output_file - Nombre del fichero de salida
 */
void AlphabetOption(const std::vector<InputLine>& entradas,
                    const std::string& output_file) {
  ProcessEntries(entradas, output_file,
                 [](const Chain& cadena, std::ostream& out, int index) {
                   out << kSigma << convertirASubindice(index) << " = "
                       << cadena.GetAlphabet() << std::endl;
                 });
}

/**
 * @brief Opción para calcular y guardar las longitudes de las cadenas
 * @param entradas - Vector de entradas con cadenas y alfabetos
 * @param output_file - Nombre del fichero de salida
 */
void LengthOption(const std::vector<InputLine>& entradas,
                  const std::string& output_file) {
  ProcessEntries(entradas, output_file,
                 [](const Chain& cadena, std::ostream& out, int index) {
                   out << "Longitud de " << kOmega << convertirASubindice(index)
                       << " = " << cadena.Length() << std::endl;
                 });
}

/**
 * @brief Opción para calcular y guardar las inversas de las cadenas
 * @param entradas - Vector de entradas con cadenas y alfabetos
 * @param output_file - Nombre del fichero de salida
 */
void InverseOption(const std::vector<InputLine>& entradas,
                   const std::string& output_file) {
  ProcessEntries(entradas, output_file,
                 [](const Chain& cadena, std::ostream& out, int) {
                   Chain inversa = cadena.Inverse();
                   out << inversa << std::endl;
                 });
}

/**
 * @brief Opción para calcular y guardar los prefijos de las cadenas
 * @param entradas - Vector de entradas con cadenas y alfabetos
 * @param output_file - Nombre del fichero de salida
 */
void PrefixOption(const std::vector<InputLine>& entradas,
                  const std::string& output_file) {
  ProcessEntries(entradas, output_file,
                 [](const Chain& cadena, std::ostream& out, int index) {
                   Language prefijos = cadena.Prefix();
                   out << "Prefijos de " << kOmega << convertirASubindice(index)
                       << ": " << prefijos << std::endl;
                 });
}

/**
 * @brief Opción para calcular y guardar los sufijos de las cadenas
 * @param entradas - Vector de entradas con cadenas y alfabetos
 * @param output_file - Nombre del fichero de salida
 */
void SuffixOption(const std::vector<InputLine>& entradas,
                  const std::string& output_file) {
  ProcessEntries(entradas, output_file,
                 [](const Chain& cadena, std::ostream& out, int index) {
                   Language sufijos = cadena.Suffix();
                   out << "Sufijos de " << kOmega << convertirASubindice(index)
                       << ": " << sufijos << std::endl;
                 });
}

/**
 * @brief Opción para calcular y guardar las subcadenas de las cadenas
 * @param entradas - Vector de entradas con cadenas y alfabetos
 * @param output_file - Nombre del fichero de salida
 */
void SubstringOption(const std::vector<InputLine>& entradas,
                     const std::string& output_file) {
  ProcessEntries(entradas, output_file,
                 [](const Chain& cadena, std::ostream& out, int index) {
                   Language subcadenas = cadena.Substring();
                   out << "Subcadenas de " << kOmega
                       << convertirASubindice(index) << ": " << subcadenas
                       << std::endl;
                 });
}

/**
 * @brief Opción para calcular y guardar las subsecuencias de las cadenas
 * @param entradas - Vector de entradas con cadenas y alfabetos
 * @param output_file - Nombre del fichero de salida
 */
void SubsequenceOption(const std::vector<InputLine>& entradas,
                       const std::string& output_file) {
  ProcessEntries(entradas, output_file,
                 [](const Chain& cadena, std::ostream& out, int index) {
                   Language subsecuencias = cadena.Subsequence();
                   out << "Subsecuencias de " << kOmega
                       << convertirASubindice(index) << ": " << subsecuencias
                       << std::endl;
                 });
}

/**
 * @brief Opción para calcular la potencia de los sufijos de las cadenas
 * @param entradas - Vector de entradas con cadenas y alfabetos
 * @param output_file - Nombre del fichero de salida
 */
void ModificationOption(const std::vector<InputLine>& entradas,
                        const std::string& output_file) {
  ProcessEntries(
      entradas, output_file,
      [](const Chain& cadena, std::ostream& out, int index) {
        std::cout << "Introduce la potencia (0-4) deseada para los sufijos de "
                  << cadena.GetChain() << ": ";
        int potencia;
        std::cin >> potencia;
        while (potencia < 0 || potencia > 4) {
          std::cout << "Potencia inválida. Introduce un número entre 0 y 4: ";
          std::cin >> potencia;
        }
        Language sufixpower = cadena.Modification(potencia);
        out << "Potencia de los sufijos de " << kOmega
            << convertirASubindice(index) << ": " << sufixpower << std::endl;
      });
}
