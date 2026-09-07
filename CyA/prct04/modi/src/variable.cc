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
 * @file variable.cc
 * @brief Fichero que contiene las funciones para el análisis de variables
 *
 * Historial de versiones
 *   03/10/2025
 *     - Creación (primera versión) del código
 *     - Sobrecarga del operador de salida para la clase Comment
 */

#include "../lib/variable.h"

#include <regex>

/**
 * @brief Función estática para recortar espacios en blanco al inicio y al final
 * de una cadena
 * @param string_clean Cadena a recortar
 * @return Cadena recortada
 */
static std::string Trim(const std::string& string_clean) {
  size_t start = string_clean.find_first_not_of(" \t");
  if (start == std::string::npos) return "";
  size_t end = string_clean.find_last_not_of(" \t");
  return string_clean.substr(start, end - start + 1);
}

// Common regex to match a single declarator like: "name", "name = 0" or
// "name{0}"
static const std::regex single_decl_regex(
    R"(^([a-zA-Z_]\w*)\s*(?:=\s*([^,;]+)|\{\s*([^}]*)\s*\})?$)");

/**
 * @brief Función estática para detectar declaraciones de variables en una línea
 * de código
 * @param line Línea de código a analizar
 * @param result Objeto Variable que almacenará el resultado
 * @param line_number Número de línea del código
 * @return true si se encuentra una declaración de variable, false en caso
 * contrario
 */
bool Variable::Match(const std::string& line, Variable& result,
                     int line_number) {
  // ignorar cabecera for
  if (line.find("for(") != std::string::npos ||
      line.find("for (") != std::string::npos) {
    return false;
  }

  static const std::regex decl_regex(R"(^\s*(int|double)\s+([^;]+)\s*;)");
  std::smatch match;
  if (!std::regex_match(line, match, decl_regex)) return false;

  std::string type = match[1].str();
  std::string declarator = Trim(match[2].str());

  std::smatch dmatch;
  if (std::regex_match(declarator, dmatch, single_decl_regex)) {
    std::string name = dmatch[1].str();
    bool init = (dmatch[2].matched || dmatch[3].matched);
    std::string init_val;

    if (dmatch[2].matched)
      init_val = "= " + Trim(dmatch[2].str());
    else if (dmatch[3].matched)
      init_val = "= " + Trim(dmatch[3].str());

    VariableType var_type;
    if (type == "int")
      var_type = VariableType::INT;
    else if (type == "double")
      var_type = VariableType::DOUBLE;
    else
      var_type = VariableType::UNKNOWN;
    result = Variable(var_type, name, line_number, init, init_val);
    return true;
  }

  return false;
}

bool Variable::MatchMultiple(const std::string& line,
                             std::vector<Variable>& results, int line_number) {
  // Local Regex to match multiple declarators in a single line: "type name1,
  // name2 = value, name3{value};"
  static const std::regex multiple_decl_regex(
      R"(^\s*(int|double)\s+([a-zA-Z_]\w*(?:\s*(?:=\s*[^,;]+|\{\s*[^}]*\s*\}))?(?:\s*,\s*[a-zA-Z_]\w*(?:\s*(?:=\s*[^,;]+|\{\s*[^}]*\s*\}))?)*)\s*;)");

  std::smatch match;
  if (!std::regex_match(line, match, multiple_decl_regex)) return false;

  std::string type = match[1].str();
  std::string list = match[2].str();

  // split by commas
  std::regex comma_re(R"(,)");
  std::sregex_token_iterator tok(list.begin(), list.end(), comma_re, -1);
  std::sregex_token_iterator tok_end;
  for (; tok != tok_end; ++tok) {
    std::string decl = Trim(tok->str());
    if (decl.empty()) continue;

    std::smatch dmatch;
    if (std::regex_match(decl, dmatch, single_decl_regex)) {
      std::string name = dmatch[1].str();
      bool init = (dmatch[2].matched || dmatch[3].matched);
      std::string init_val;
      if (dmatch[2].matched)
        init_val = "= " + Trim(dmatch[2].str());
      else if (dmatch[3].matched)
        init_val = "= " + Trim(dmatch[3].str());

      VariableType vtype =
          (type == "int") ? VariableType::INT : VariableType::DOUBLE;
      results.emplace_back(vtype, name, line_number, init, init_val);
    }
  }

  return !results.empty();
}

/**
 * @brief Sobrecarga del operador de salida para la clase Variable
 * @param os - ostream
 * @param variable - Objeto de la clase Variable
 * @return os - ostream modificado
 */
std::ostream& operator<<(std::ostream& os, const Variable& variable) {
  std::string type;
  switch (variable.type_) {
    case VariableType::INT:
      type = "INT";
      break;
    case VariableType::DOUBLE:
      type = "DOUBLE";
      break;
    default:
      type = "UNKNOWN";
      break;
  }
  os << "[Line " << variable.line_ << "] " << type << " " << variable.name_;
  if (variable.initialized_) {
    // initial_value_ already normalized to include a leading "= " when present
    os << " " << variable.initial_value_;
  } else {
    os << " (uninitialized)";
  }
  os << std::endl;
  return os;
}