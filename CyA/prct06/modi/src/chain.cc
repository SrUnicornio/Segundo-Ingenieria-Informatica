/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 6: Autómatas Finitos
 *
 * @author 
 * @date 15/10/2025
 * @file chain.cc
 * @brief Fichero que contiene las declaraciones de los métodos de la clase
 * Chain
 *
 * Historial de versiones
 *   17/09/2025
 *     - Creación (primera versión) del código
 *     - Implementación de métodos de la clase
 *     - Sobrecarga de operadores y control de errores
 *  15/10/2025
 *     - Actualización de la documentación y eliminación de metodos innecesarios
 */

#include "../lib/chain.h"

/**
 * @brief Constructor de la clase Chain.
 * @param chain - La cadena de símbolos.
 * @param alphabet - El alfabeto al que pertenece la cadena.
 * @throw EmptySymbol Si la cadena contiene el símbolo vacío '&'.
 * @throw Error Si la cadena contiene símbolos que no pertenecen al alfabeto.
 */
Chain::Chain(std::string chain, const Alphabet& alphabet)
    : chain_(chain), alphabet_(alphabet) {
  if (HasEmptySymbol()) {
    throw EmptySymbol("Error: La cadena '" + chain +
                      "' contiene el símbolo vacío '&'.");
  }
  if (!IsInAlphabet()) {
    throw ChainNotInAlphabet(
        "Error: La cadena '" + chain +
        "' contiene símbolos que no pertenecen al alfabeto ");
  }
  if (chain.empty() || chain == kVoidChainSymbol) {
    chain_ = kVoidChainSymbol;
  }
}

/**
 * @brief Método que dice si todos los símbolos de la cadena pertenecen al
 * alfabeto.
 * @return true si todos los símbolos de la cadena pertenecen al alfabeto, false
 * en caso contrario.
 */
bool Chain::IsInAlphabet() const {
  const auto& allowed = alphabet_.GetAlphabet();
  for (char ch : chain_) {
    if (ch == '&') {
      continue;
    }
    if (allowed.find(Symbol(ch)) == allowed.end()) {
      return false;
    }
  }
  return true;  
}

/**
 * @brief Método que dice si la cadena contiene el símbolo vacío '&'.
 * @return true si la cadena contiene el símbolo vacío '&', false en caso
 * contrario.
 */
bool Chain::HasEmptySymbol() const {
  return chain_.length() > 1 && chain_.find(kVoidChainSymbol) != std::string::npos;
}

/**
 * @brief Sobrecarga del operador de salida para la clase Chain.
 * @param os - flujo de salida
 * @param chain - cadena a imprimir
 * @return flujo de salida
 */
std::ostream& operator<<(std::ostream& os, const Chain& chain) {
  os << chain.chain_;
  return os;
}

/**
 * @brief Sobrecarga del operador de entrada para la clase Chain.
 * @param is - flujo de entrada
 * @param chain - cadena a leer
 * @return flujo de entrada
 */
std::istream& operator>>(std::istream& is, Chain& chain) {
  is >> chain.chain_;
  return is;
}
