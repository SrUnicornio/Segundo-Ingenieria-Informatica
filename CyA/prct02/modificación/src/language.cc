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
 * @file language.h
 * @brief Fichero que contiene la definición de la clase Language
 */

#include "../lib/language.h"

/**
 * @brief Destructor que limpia el lenguaje.
 */
void Language::Clear() { language_set_.clear(); }

/**
 * @brief Elimina una cadena del lenguaje.
 * @param chain - cadena a eliminar
 */
void Language::Remove(const Chain& chain) {
  if (language_set_.find(chain) == language_set_.end()) {
    std::cerr << "Warning: La cadena '" << chain
              << "' no existe en el lenguaje." << std::endl;
    return;
  }
  language_set_.erase(chain);
}

/**
 * @brief Calcula la concatenación de dos lenguajes.
 * @param other - otro lenguaje
 * @return nuevo lenguaje que es la concatenación de los dos lenguajes
 */
Language Language::Concatenate(const Language& other) const {
  Language result;
  for (const auto& c1 : language_set_) {
    for (const auto& c2 : other.language_set_) {
      result.Insert(c1.Concatenate(c2));
    }
  }
  return result;
}

/**
 * @brief Calcula la potencia del lenguaje.
 * @param number - potencia a la que elevar el lenguaje 
 * @return 
 */
Language Language::Power(size_t number) const {
  if (number == 0) return Language(Chain(kVoidChainSymbol, alphabet_));
  if (number == 1) return *this;

  Language result = *this;
  for (size_t i = 1; i < number; ++i) {
    result = result.Concatenate(*this);
  }
  return result;
}

/**
 * @brief Calcula la unión entre dos lenguajes.
 * @param other - otro lenguaje
 * @return nuevo lenguaje que es la unión de los dos lenguajes
 */
Language Language::Union(const Language& other) const {
  Language result = *this;
  for (const auto& c : other.language_set_) {
    result.Insert(c);
  }
  return result;
}

/**
 * @brief Calcula la intersección entre dos lenguajes.
 * @param other - otro lenguaje
 * @return nuevo lenguaje que es la intersección de los dos lenguajes
 */
Language Language::Intersection(const Language& other) const {
  Language result;
  for (const auto& c : language_set_) {
    if (other.Contains(c)) {
      result.Insert(c);
    }
  }
  return result;
}

/**
 * @brief Comprueba si el lenguaje actual es subconjunto de otro.
 * @param other - otro lenguaje
 * @return true si el lenguaje actual es subconjunto de otro, false en caso contrario
 */
bool Language::IsSubsetOf(const Language& other) const {
  for (const auto& c : language_set_) {
    if (!other.Contains(c)) return false;
  }
  return true;
}

/**
 * @brief Sobrecarga del operador de igualdad.
 * @param other - otro lenguaje
 * @return true si los lenguajes son iguales, false en caso contrario
 */
bool Language::operator==(const Language& other) const {
  return language_set_ == other.language_set_;
}

/**
 * @brief Calcula el cierre de Kleene del lenguaje hasta una potencia máxima.
 * @param max_power - potencia máxima
 * @return nuevo lenguaje que es el cierre de Kleene del lenguaje actual
 */
Language Language::Kleene(size_t max_power) const {
  Language result(Chain(kVoidChainSymbol, alphabet_));
  for (size_t i = 1; i <= max_power; ++i) {
    result = result.Union(this->Power(i));
  }
  return result;
}

/**
 * @brief Calcula el cierre positivo del lenguaje hasta una potencia máxima.
 * @param max_power - potencia máxima
 * @return nuevo lenguaje que es el cierre positivo del lenguaje actual
 */
Language Language::Positive(size_t max_power) const {
  Language result = *this;
  for (size_t i = 2; i <= max_power; ++i) {
    result = result.Union(this->Power(i));
  }
  return result;
}

/**
 * @brief Calcula la diferencia entre dos lenguajes.
 * @param other - otro lenguaje
 * @return nuevo lenguaje que es la diferencia entre el lenguaje actual y el otro
 */
Language Language::Difference(const Language& other) const {
  Language result;
  for (const auto& c : language_set_) {
    if (!other.Contains(c)) {
      result.Insert(c);
    }
  }
  return result;
}

/**
 * @brief Sobrecarga del operador de salida para imprimir el lenguaje.
 * @param os - flujo de salida
 * @param language - lenguaje a imprimir
 * @return flujo de salida
 */
std::ostream& operator<<(std::ostream& os, const Language& language) {
  os << "{";
  bool first = true;
  for (const auto& chain : language.language_set_) {
    if (!first) {
      os << ", ";
    }
    os << chain;
    first = false;
  }
  os << "}";
  return os;
}

/**
 * @brief Sobrecarga del operador de entrada para leer un lenguaje.
 * Formato esperado: número de cadenas seguido de cada cadena.
 * @param is - flujo de entrada
 * @param language - lenguaje a leer
 * @return flujo de entrada
 */
std::istream& operator>>(std::istream& is, Language& language) {
  size_t n;
  is >> n;
  for (size_t i = 0; i < n; ++i) {
    std::string chain_str;
    is >> chain_str;
    Chain chain(chain_str, Alphabet(chain_str));
    language.Insert(chain);
  }
  return is;
}