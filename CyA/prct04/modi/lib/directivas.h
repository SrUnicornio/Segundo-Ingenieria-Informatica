/**
 * @file directivas.h
 * @brief Declaración de la clase Directivas para detectar directivas del
 * preprocesador (#include, #define, #pragma, #ifndef/#endif)
 */

#ifndef DIRECTIVAS_H_
#define DIRECTIVAS_H_

#include <iostream>
#include <string>

enum class DirectivasType {
  INCLUDES_STL,  // #include <...>
  INCLUDES_USR,  // #include "..."
  DEFINE,        // #define ...
  PRAGMA,        // #pragma ...
  IFNDEF_ENDIF   // #ifndef ... (accumulates until #endif)
};

class Directivas {
 public:
  Directivas() = default;
  Directivas(DirectivasType type, int start_line, int end_line,
             const std::string& content)
      : type_(type),
        start_line_(start_line),
        end_line_(end_line),
        content_(content) {
    IncrementCounter(type_);
  }
  ~Directivas() = default;

  DirectivasType GetType() const { return type_; }
  int GetStartLine() const { return start_line_; }
  int GetEndLine() const { return end_line_; }
  std::string GetContent() const { return content_; }

  static bool Match(const std::string& line, Directivas& result,
                    int line_number, bool& inside_multiline,
                    std::string& multi_content, int& multi_start_line);

  static void IncrementCounter(DirectivasType type);
  static void PrintSummary(std::ostream& os = std::cout);

  friend std::ostream& operator<<(std::ostream& os,
                                  const Directivas& directive);

 private:
  DirectivasType type_ = DirectivasType::INCLUDES_STL;
  int start_line_ = 0;
  int end_line_ = 0;
  std::string content_ = "";

  // Contadores estáticos
  static inline int count_includes_stl_ = 0;
  static inline int count_includes_usr_ = 0;
  static inline int count_define_ = 0;
  static inline int count_pragma_ = 0;
  static inline int count_ifndef_endif_ = 0;
};

#endif  // DIRECTIVAS_H_
