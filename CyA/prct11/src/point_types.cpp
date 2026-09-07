/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 11: Algoritmos Voraces
 * Autor: 
 * Correo: alu0101574042@ull.edu.es
 * Fecha: 03/12/2025
 * Archivo: point_types.cpp - Implementación de las funciones auxiliares
 */
#include "point_types.hpp"

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <sstream>

/**
 * @brief sobrecarga del operador de inserción
 * @param os flujo de salida
 * @param points_vector vector de puntos
 * @return flujo de salida
 */
std::ostream& operator<<(std::ostream& os, const CyA::point_vector& points_vector) {
  os << points_vector.size() << std::endl;

  for (const CyA::point& points : points_vector) {
    os << points << std::endl;
  }

  return os;
}

/**
 * @brief sobrecarga del operador de inserción
 * @param os flujo de salida
 * @param points punto
 * @return flujo de salida
 */
std::ostream& operator<<(std::ostream& os, const CyA::point& points) {
  os << std::setw(MAX_SZ) << std::fixed << std::setprecision(MAX_PREC)
     << points.first << "\t" << std::setw(MAX_SZ) << std::fixed
     << std::setprecision(MAX_PREC) << points.second;

  return os;
}

/**
 * @brief sobrecarga del operador de extracción
 * @param is flujo de entrada
 * @param points_vector vector de puntos
 * @return flujo de entrada
 */
std::istream& operator>>(std::istream& is, CyA::point_vector& points_vector) {
  int num_points = 0;
  if (!(is >> num_points)) {
    throw std::runtime_error("Error: No se pudo leer el número de puntos.");
  }

  points_vector.clear();
  points_vector.reserve(num_points);

  for (int i = 0; i < num_points; ++i) {
    CyA::point p;
    if (!(is >> p.first >> p.second)) {
      std::ostringstream oss;
      oss << "Error: El archivo declara " << num_points
          << " puntos, pero solo se pudieron leer " << i << ".\n"
          << "Revisa el formato: falta un punto en la línea " << (i + 2) << ".";
      throw std::runtime_error(oss.str());
    }
    points_vector.push_back(p);
  }

  // Detectar puntos EXTRA
  double extra_x, extra_y;
  if (is >> extra_x >> extra_y) {
    throw std::runtime_error(
        "Error: El archivo contiene MÁS puntos de los declarados.\n"
        "Revisa el número inicial o elimina líneas sobrantes.");
  }

  return is;
}

/**
 * @brief sobrecarga del operador de extracción
 * @param is flujo de entrada
 * @param points punto
 * @return flujo de entrada
 */
std::istream& operator>>(std::istream& is, CyA::point& points) {
  is >> points.first >> points.second;

  return is;
}