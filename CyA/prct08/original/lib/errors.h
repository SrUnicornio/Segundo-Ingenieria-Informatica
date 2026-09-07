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
 * @file errors.h
 * @brief Definición de las excepciones usadas en la práctica
 *
 * Historial de versiones
 *   17/09/2025 - Creación de las clases de excepción
 */

#ifndef ERRORES_H_
#define ERRORES_H_

#include <stdexcept>
#include <string>

/**
 * @class Error
 * @brief Clase base para todas las excepciones personalizadas.
 */
class Error : public std::exception {
 public:
  /**
   * @brief Constructor de la excepción.
   * @param message Mensaje de error a mostrar.
   */
  explicit Error(const std::string& message) : message_(message) {}

  /**
   * @brief Devuelve el mensaje de error.
   * @return Mensaje de error como cadena C.
   */
  const char* what() const noexcept override { return message_.c_str(); }

 private:
  std::string message_;
};

// =============================================================================
// Excepciones para fichero de entrada/salida
// =============================================================================
/**
 * @class FileError
 * @brief Excepción lanzada al ocurrir un error con el fichero de entrada o
 * salida.
 */
class FileError : public Error {
 public:
  explicit FileError(const std::string& message) : Error(message) {}
};

// =============================================================================
// Excepciones para la clase Alphabet
// =============================================================================

/**
 * @class EmptySymbol
 * @brief Excepción lanzada al intentar incluir el símbolo vacío '&' en un
 * alfabeto.
 */
class EmptySymbol : public Error {
 public:
  explicit EmptySymbol(const std::string& message) : Error(message) {}
};

/**
 * @class VoidAlphabet
 * @brief Excepción lanzada al intentar crear un alfabeto vacío.
 */
class VoidAlphabet : public Error {
 public:
  explicit VoidAlphabet(const std::string& message) : Error(message) {}
};

// =============================================================================
// Excepciones para la clase Chain
// =============================================================================

/**
 * @class ChainNotInAlphabet
 * @brief Excepción lanzada cuando una cadena contiene símbolos que no
 * pertenecen al alfabeto.
 */
class ChainNotInAlphabet : public Error {
 public:
  explicit ChainNotInAlphabet(const std::string& message) : Error(message) {}
};

/**
 * @class ChainHasEmptySymbol
 * @brief Excepción lanzada cuando una cadena contiene el símbolo vacío '&'.
 */
class ChainHasEmptySymbol : public Error {
 public:
  explicit ChainHasEmptySymbol(const std::string& message) : Error(message) {}
};

// =============================================================================
// Excepciones para la clase Grammar
// =============================================================================

/**
 * @class GrammarFormatError
 * @brief Excepción lanzada si la gramática no cumple el formato esperado.
 */
class GrammarFormatError : public Error {
 public:
  explicit GrammarFormatError(const std::string& message) : Error(message) {}
};

/**
 * @class MissingStartSymbol
 * @brief Excepción lanzada si la gramática no contiene el símbolo de arranque
 * S.
 */
class MissingStartSymbol : public Error {
 public:
  explicit MissingStartSymbol(const std::string& message) : Error(message) {}
};

/**
 * @class ProductionCountMismatch
 * @brief Excepción lanzada si el número de producciones no coincide con el
 * declarado.
 */
class ProductionCountMismatch : public Error {
 public:
  explicit ProductionCountMismatch(const std::string& message)
      : Error(message) {}
};

/**
 * @class UnitaryProductionError
 * @brief Excepción lanzada si se encuentra una producción unitaria.
 */
class UnitaryProductionError : public Error {
 public:
  explicit UnitaryProductionError(const std::string& message)
      : Error(message) {}
};

/**
 * @class EmptyProductionError
 * @brief Excepción lanzada si se encuentra una producción vacía.
 */
class EmptyProductionError : public Error {
 public:
  explicit EmptyProductionError(const std::string& message)
      : Error(message) {}
};

/**
 * @class UselessSymbolError
 * @brief Excepción lanzada si se encuentra un símbolo inútil en la gramática.
 */
class UselessSymbolError : public Error {
 public:
  explicit UselessSymbolError(const std::string& message)
      : Error(message) {}
};

#endif  // ERRORES_H_
