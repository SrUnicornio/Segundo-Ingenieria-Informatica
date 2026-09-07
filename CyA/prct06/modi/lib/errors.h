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
// Excepciones para la clase Automaton
// =============================================================================

/**
 * @class InvalidState
 * @brief Excepción lanzada cuando se intenta usar un estado no definido en
 * un autómata.
 */
class InvalidState : public Error {
 public:
  explicit InvalidState(const std::string& message) : Error(message) {}
};

/**
 * @class MoreThanOneInitialState
 * @brief Excepción lanzada cuando se intenta definir más de un estado inicial en
 * un autómata.
 */
class MoreThanOneInitialState : public Error {
 public:
  explicit MoreThanOneInitialState(const std::string& message) : Error(message) {}
};

/**
 * @class NoInitialState
 * @brief Excepción lanzada cuando no se define un estado inicial en un
 * autómata.
 */
class NoInitialState : public Error {
 public:
  explicit NoInitialState(const std::string& message) : Error(message) {}
};

/**
 * @class NoFinalStates
 * @brief Excepción lanzada cuando no se definen estados finales en un
 * autómata.
 */
class NoFinalStates : public Error {
 public:
  explicit NoFinalStates(const std::string& message) : Error(message) {}
};

/**
 * @class TransitionNotInAlphabet
 * @brief Excepción lanzada cuando una transición contiene un símbolo que no
 * pertenece al alfabeto del autómata.
 */
class TransitionNotInAlphabet : public Error {
 public:
  explicit TransitionNotInAlphabet(const std::string& message) : Error(message) {}
};

/**
 * @class NonReachableState
 * @brief Excepción lanzada cuando un estado no es alcanzable desde el estado
 * inicial.
 */
class NonReachableState : public Error {
 public:
  explicit NonReachableState(const std::string& message) : Error(message) {}
};

// =============================================================================
// Excepciones para validación de autómatas
// =============================================================================

/**
 * @class LessStatesThanDefined
 * @brief Excepción lanzada cuando el número de estados definidos es menor que
 * el número declarado.
 */
class LessStatesThanDefined : public Error {
 public:
  explicit LessStatesThanDefined(const std::string& message) : Error(message) {}
};
#endif  // ERRORES_H_
