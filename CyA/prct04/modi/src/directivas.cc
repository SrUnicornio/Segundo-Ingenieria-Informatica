/**
 * @file directivas.cc
 * @brief Implementación de la detección de directivas del preprocesador
 */

#include "../lib/directivas.h"

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

// Detecta directivas sencillas por línea y bloques #ifndef ... #endif
bool Directivas::Match(const std::string& line, Directivas& result,
                       int line_number, bool& inside_multiline,
                       std::string& multi_content, int& multi_start_line) {
  static const std::regex include_stl("^\\s*#\\s*include\\s*<([^>]+)>");
  static const std::regex include_usr("^\\s*#\\s*include\\s*\"([^\"]+)\"");
  static const std::regex define_re("^\\s*#\\s*define\\b(.*)");
  static const std::regex pragma_re("^\\s*#\\s*pragma\\b(.*)");
  static const std::regex ifndef_re("^\\s*#\\s*ifndef\\b(.*)");
  static const std::regex endif_re("^\\s*#\\s*endif\\b(.*)");

  std::smatch m;
  if (std::regex_search(line, m, include_stl)) {
    result = Directivas(DirectivasType::INCLUDES_STL, line_number, line_number,
                        m[1].str());
    return true;
  }
  if (std::regex_search(line, m, include_usr)) {
    result = Directivas(DirectivasType::INCLUDES_USR, line_number, line_number,
                        m[1].str());
    return true;
  }
  if (std::regex_search(line, m, define_re)) {
    result = Directivas(DirectivasType::DEFINE, line_number, line_number,
                        Trim(m[1].str()));
    return true;
  }
  if (std::regex_search(line, m, pragma_re)) {
    result = Directivas(DirectivasType::PRAGMA, line_number, line_number,
                        m[1].str());
    return true;
  }

  // Bloque #ifndef ... #endif
  if (!inside_multiline) {
    if (std::regex_search(line, m, ifndef_re)) {
      // store only the macro/name after #ifndef; do not accumulate body
      inside_multiline = true;
      multi_start_line = line_number;
      multi_content = Trim(m[1].str());
      return false;  // still waiting for #endif
    }
  } else {
    // estamos dentro de un bloque #ifndef ... esperar #endif
    if (std::regex_search(line, m, endif_re)) {
      inside_multiline = false;
      // Create directive using only the macro name stored in multi_content
      result = Directivas(DirectivasType::IFNDEF_ENDIF, multi_start_line,
                          line_number, multi_content);
      multi_content.clear();
      multi_start_line = 0;
      return true;
    } else {
      // ignore inner lines; we're only interested in the block boundaries
      return false;
    }
  }

  return false;
}

// Increment counters
void Directivas::IncrementCounter(DirectivasType type) {
  switch (type) {
    case DirectivasType::INCLUDES_STL:
      ++count_includes_stl_;
      break;
    case DirectivasType::INCLUDES_USR:
      ++count_includes_usr_;
      break;
    case DirectivasType::DEFINE:
      ++count_define_;
      break;
    case DirectivasType::PRAGMA:
      ++count_pragma_;
      break;
    case DirectivasType::IFNDEF_ENDIF:
      ++count_ifndef_endif_;
      break;
  }
}

void Directivas::PrintSummary(std::ostream& os) {
  os << "DIRECTIVAS SUMMARY:\n";
  os << "INCLUDES_STL: " << count_includes_stl_ << "\n";
  os << "INCLUDES_USR: " << count_includes_usr_ << "\n";
  os << "DEFINE: " << count_define_ << "\n";
  os << "PRAGMA: " << count_pragma_ << "\n";
  os << "IFNDEF_ENDIF: " << count_ifndef_endif_ << "\n";
}

std::ostream& operator<<(std::ostream& os, const Directivas& directive) {
  switch (directive.type_) {
    case DirectivasType::INCLUDES_STL:
      os << "[Line " << directive.start_line_ << "] INCLUDE <"
         << directive.content_ << ">\n";
      break;
    case DirectivasType::INCLUDES_USR:
      os << "[Line " << directive.start_line_ << "] INCLUDE \""
         << directive.content_ << "\"\n";
      break;
    case DirectivasType::DEFINE:
      os << "[Line " << directive.start_line_ << "] DEFINE "
         << directive.content_ << "\n";
      break;
    case DirectivasType::PRAGMA:
      os << "[Line " << directive.start_line_ << "] PRAGMA "
         << directive.content_ << "\n";
      break;
    case DirectivasType::IFNDEF_ENDIF:
      os << "[Lines " << directive.start_line_ << "-" << directive.end_line_
         << "] IFNDEF/ENDIF " << directive.content_ << "\n";
      break;
  }
  return os;
}
