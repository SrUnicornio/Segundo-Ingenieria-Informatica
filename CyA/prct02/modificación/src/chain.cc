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
 * @file chain.cc
 * @brief Fichero que contiene las declaraciones de los métodos de la clase
 * Chain
 *
 * Historial de versiones
 *   17/09/2025
 *     - Creación (primera versión) del código
 *     - Implementación de métodos de la clase
 *     - Sobrecarga de operadores y control de errores
 */

#include "../lib/chain.h"
#include "../lib/language.h"

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
  return chain_.length() > 1 && chain_.find('&') != std::string::npos;
}

/**
 * @brief Método que devuelve la longitud de la cadena.
 * @return La longitud de la cadena.
 */
int Chain::Length() const { return chain_ == kVoidChainSymbol ? 0 : chain_.size(); }

/**
 * @brief Método que devuelve la cadena inversa.
 * @return La cadena inversa.
 */
Chain Chain::Inverse() const {
  if (chain_ == kVoidChainSymbol) {
    return Chain(kVoidChainSymbol, alphabet_);
  }
  std::string reversed_chain = chain_;
  for (size_t i = 0, j = reversed_chain.size() - 1; i < j; ++i, --j) {
    std::swap(reversed_chain[i], reversed_chain[j]);
  }
  return Chain(reversed_chain, alphabet_);
}

/**
 * @brief Método que devuelve los prefijos de una cadena.
 * @return Un conjunto con los prefijos de la cadena.
 */
Language Chain::Prefix() const {
  Language prefixes;
  prefixes.Insert(Chain(kVoidChainSymbol, GetAlphabet()));
  if (chain_ != kVoidChainSymbol) {
    for (size_t i = 1; i <= chain_.size(); ++i) {
      std::string pre = chain_.substr(0, i);
      prefixes.Insert(Chain(pre, GetAlphabet()));
    }
  }
  return prefixes;
}

/**
 * @brief Método que devuelve los sufijos de una cadena.
 * @return Un conjunto con los sufijos de la cadena.
 */
Language Chain::Suffix() const {
  Language suffixes;
  suffixes.Insert(Chain(kVoidChainSymbol, GetAlphabet()));
  if (chain_ != kVoidChainSymbol) {
    for (size_t i = 0; i < chain_.size(); ++i) {
      std::string suf = chain_.substr(i);
      suffixes.Insert(Chain(suf, GetAlphabet()));
    }
  }
  return suffixes;
}

/**
 * @brief Método que devuelve el prefijo común más largo entre dos cadenas.
 * @param other - La otra cadena con la que comparar.
 * @return El prefijo común más largo entre las dos cadenas.
 */
Chain Chain::CommonPrefix(const Chain& other) const {
  size_t len = std::min(chain_.size(), other.chain_.size());
  size_t i = 0;
  while (i < len && chain_[i] == other.chain_[i]) {
    ++i;
  }
  if (i == 0) {
    return Chain(kVoidChainSymbol, alphabet_);
  }
  return Chain(chain_.substr(0, i), alphabet_);
}

/**
 * @brief Método que devuelve el sufijo común más largo entre dos cadenas.
 * @param other - La otra cadena con la que comparar.
 * @return El sufijo común más largo entre las dos cadenas.
 */
Chain Chain::CommonSuffix(const Chain& other) const {
  size_t len1 = chain_.size();
  size_t len2 = other.chain_.size();
  size_t i = 0;
  while (i < len1 && i < len2 &&
         chain_[len1 - 1 - i] == other.chain_[len2 - 1 - i]) {
    ++i;
  }
  if (i == 0) {
    return Chain(kVoidChainSymbol, alphabet_);
  }
  return Chain(chain_.substr(len1 - i), alphabet_);
}

/**
 * @brief Método que devuelve las subcadenas de una cadena.
 * @return Un conjunto con las subcadenas de la cadena.
 */
Language Chain::Substring() const {
  Language substrings;
  substrings.Insert(Chain(kVoidChainSymbol, alphabet_));  // incluir la cadena vacía
  if (chain_ == kVoidChainSymbol) {
    return substrings;
  }
  int longitud = chain_.size();
  for (int i = 0; i < longitud; ++i) {
    for (int j = i + 1; j <= longitud; ++j) {
      std::string sub = chain_.substr(i, j - i);
      substrings.Insert(Chain(sub, alphabet_));
    }
  }
  return substrings;
}

/**
 * @brief Método que devuelve las subsecuencias de una cadena.
 * @return Un conjunto con las subsecuencias de la cadena.
 */
Language Chain::Subsequence() const {
  Language subsequences;
  subsequences.Insert(Chain(kVoidChainSymbol, alphabet_));
  if (chain_ == kVoidChainSymbol) {
    return subsequences;
  }
  int longitud = chain_.size();
  for (int i = 0; i < (1 << longitud); ++i) {
    std::string subseq;
    for (int j = 0; j < longitud; ++j) {
      if (i & (1 << j)) {
        subseq += chain_[j];
      }
    }
    subsequences.Insert(Chain(subseq, alphabet_));
  }
  return subsequences;
}

/**
 * @brief Método que concatena dos cadenas.
 * @param other - La otra cadena a concatenar.
 * @return La nueva cadena resultante de la concatenación.
 */
Chain Chain::Concatenate(const Chain& other) const {
  if (chain_ == kVoidChainSymbol) {
    return Chain(other.chain_, alphabet_);
  }
  if (other.chain_ == kVoidChainSymbol) {
    return Chain(chain_, alphabet_);
  }
  return Chain(chain_ + other.chain_, alphabet_);
}

/** @brief Método que repite la cadena n veces.
 * @param number_repetitions - Número de repeticiones.
 * @return La nueva cadena resultante de las repeticiones.
 * @throw InvalidOperation Si n es negativo.
 */
Chain Chain::Repeat(int number_repetitions) const {
  if (number_repetitions < 0) {
    throw InvalidOperation("Error: El número de repeticiones no puede ser "
                                "negativo.");
  }
  if (number_repetitions == 0 || chain_ == kVoidChainSymbol) {
    return Chain(kVoidChainSymbol, alphabet_);
  }
  Chain repeated_chain = Chain(chain_, alphabet_);
  for (int i = 0; i < number_repetitions; ++i) {
    repeated_chain = repeated_chain.Concatenate(*this);
  }
  return repeated_chain;
}

/**
 * @brief Método que comprueba si dos cadenas son iguales.
 * @param other - La otra cadena con la que comparar.
 * @return true si las cadenas son iguales, false en caso contrario.
 */
bool Chain::IsEqual(const Chain& other) const { return chain_ == other.chain_; }

/**
 * @brief Método que comprueba si la cadena es una subsecuencia de otra.
 * @param other - La otra cadena con la que comparar.
 * @return true si la cadena es una subsecuencia de la otra, false en caso
 * contrario.
 */
bool Chain::IsSubsequence(const Chain& other) const {
  size_t i = 0, j = 0;
  const std::string& s1 = chain_;
  const std::string& s2 = other.chain_;

  while (i < s1.size() && j < s2.size()) {
    if (s1[i] == s2[j]) {
      ++i;
    }
    ++j;
  }
  return i == s1.size();
}

/**
 * @brief Método que comprueba si la cadena es una subcadena (substring) de
 * otra.
 * @param other - La otra cadena con la que comparar.
 * @return true si la cadena es una subcadena de la otra, false en caso
 * contrario.
 */
bool Chain::IsSubstring(const Chain& other) const {
  const std::string& s1 = chain_;
  const std::string& s2 = other.chain_;
  return s2.find(s1) != std::string::npos;
}

/**
 * @brief Método que calcula las modificaciones de la cadena con un máximo de
 * modificaciones.
 * @param max_modifications - Número máximo de modificaciones permitidas.
 * @return Un conjunto con las modificaciones de la cadena.
 */
Language Chain::Modification(size_t max_modifications) const {
  Language suffixes = this->Suffix();
  Language modifications = suffixes.Power(max_modifications);
  return modifications;
}

/**
 * @brief Sobrecarga del operador de comparación menor que.
 * @param Otra cadena con la que comparar.
 * @return true si la cadena actual es menor que.
 */
bool Chain::operator<(const Chain& other) const {
  if (Length() == other.Length()) {
    return chain_ < other.chain_;  // si miden lo mismo, orden alfabético
  }
  return Length() < other.Length();  // si no, orden por longitud
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
