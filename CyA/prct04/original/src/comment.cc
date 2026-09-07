/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 4: Code analyzer
 * @author 
 * @date 03/10/2025
 * @file comment.h
 * @brief Fichero que contiene las funciones para el análisis de comentarios
 *
 * Historial de versiones
 *   03/10/2025
 *     - Creación (primera versión) del código
 *     - Sobrecarga del operador de salida para la clase Comment
 */
#include "../lib/comment.h"

#include <regex>

/**
 * @brief Función estática para encontrar un delimitador en un texto que no esté
 * dentro de una cadena entre comillas dobles.
 * @param text Texto en el que buscar el delimitador
 * @param delimiter Delimitador a buscar
 * @return Índice del delimitador o -1 si no se encuentra
 */
static int FindDelimiterOutsideString(const std::string& text,
                                      const std::string& delimiter) {
  bool inside_string_literal = false;
  bool previous_char_was_escape = false;
  size_t text_len = text.size();
  size_t delim_len = delimiter.size();
  for (size_t i = 0; i + delim_len <= text_len; ++i) {
    char ch = text[i];
    if (ch == '\\' && !previous_char_was_escape) {
      previous_char_was_escape = true;
      continue;
    }
    if (ch == '"' && !previous_char_was_escape)
      inside_string_literal = !inside_string_literal;
    if (!inside_string_literal && !previous_char_was_escape) {
      bool matches = true;
      for (size_t j = 0; j < delim_len; ++j) {
        if (text[i + j] != delimiter[j]) {
          matches = false;
          break;
        }
      }
      if (matches) return static_cast<int>(i);
    }
    previous_char_was_escape = false;
  }
  return -1;
}

/**
 * @brief Función estática para detectar comentarios en una línea de código
 * @param line Línea de código a analizar
 * @param result Objeto Comment que almacenará el resultado
 * @param line_number Número de línea del código
 * @param inside_multiline Booleano que indica si se está dentro de un
 * comentario multilínea
 * @param multi_content Contenido acumulado del comentario multilínea
 * @param multi_start_line Línea de inicio del comentario multilínea
 * @return true si se encuentra un comentario, false en caso contrario
 */
bool Comment::Match(const std::string& line, Comment& result, int line_number,
                    bool& inside_multiline, std::string& multi_content,
                    int& multi_start_line) {
  static const std::regex multi_end(R"(\*/)");

  // If we're not inside a multiline comment, try to detect a single-line
  // comment '//' outside of string literals.
  if (!inside_multiline) {
    int pos = FindDelimiterOutsideString(line, "//");
    if (pos >= 0) {
      std::string comment_text = line.substr(static_cast<size_t>(pos));
      result = Comment(CommentType::SINGLE_LINE, line_number, line_number,
                       comment_text);
      return true;
    }

    // Try start of multiline '/*' outside strings
    int mpos = FindDelimiterOutsideString(line, "/*");
    if (mpos >= 0) {
      inside_multiline = true;
      multi_start_line = line_number;
      multi_content = line.substr(static_cast<size_t>(mpos)) + "\n";
      // If the same line also contains the end '*/', close it immediately
      if (std::regex_search(line, multi_end)) {
        inside_multiline = false;
        CommentType type = (multi_start_line == 1) ? CommentType::DESCRIPTION
                                                   : CommentType::MULTI_LINE;
        result = Comment(type, multi_start_line, line_number, multi_content);
        return true;
      }
      return false;
    }
  }

  // continuation / end of an already-open multiline comment
  if (inside_multiline) {
    multi_content += line + "\n";
    if (std::regex_search(line, multi_end)) {
      inside_multiline = false;
      CommentType type = (multi_start_line == 1) ? CommentType::DESCRIPTION
                                                 : CommentType::MULTI_LINE;
      result = Comment(type, multi_start_line, line_number, multi_content);
      return true;
    }
  }

  return false;
}

/**
 * @brief Sobrecarga del operador de salida para la clase Comment
 * @param os - ostream
 * @param comment - Objeto de la clase Comment
 * @return os - ostream modificado
 */
std::ostream& operator<<(std::ostream& os, const Comment& comment) {
  std::string content = comment.content_;
  int start_line = comment.start_line_;
  int end_line = comment.end_line_;
  switch (comment.type_) {
    case CommentType::DESCRIPTION:
      os << "[Line " << start_line << "-" << end_line << "] DESCRIPTION"
         << std::endl;
      break;
    case CommentType::SINGLE_LINE:
      os << "[Line " << start_line << "] " << content << std::endl;
      break;
    case CommentType::MULTI_LINE:
      os << "[Line " << start_line << "-" << end_line << "] " << content
         << std::endl;
      break;
  }
  return os;
}