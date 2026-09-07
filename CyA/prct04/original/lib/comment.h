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
 *     - Añadida la clase Comment para representar comentarios en el código
 */

#ifndef COMMENT_H_
#define COMMENT_H_

#include <iostream>
#include <string>

enum class CommentType { SINGLE_LINE, MULTI_LINE, DESCRIPTION };

class Comment {
 public:
  // Constructor por defecto
  Comment() = default;
  Comment(CommentType type, int start_line, int end_line,
          const std::string& content)
      : type_(type),
        start_line_(start_line),
        end_line_(end_line),
        content_(content) {}
  ~Comment() = default;
  
  CommentType GetType() const { return type_; }
  int GetStartLine() const { return start_line_; }
  int GetEndLine() const { return end_line_; }
  std::string GetContent() const { return content_; }

  static bool Match(const std::string& line, Comment& result, int line_number,
                    bool& inside_multiline, std::string& multi_content,
                    int& multi_start_line);

  friend std::ostream& operator<<(std::ostream& os, const Comment& comment);

 private:
  CommentType type_ = CommentType::SINGLE_LINE;
  int start_line_ = 0;
  int end_line_ = 0;
  std::string content_ = "";
};

#endif
