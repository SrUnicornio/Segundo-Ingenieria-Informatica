/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 12: Algoritmos Divide y Vencerás
 * Autor: 
 * Correo: alu0101574042@ull.edu.es
 * Fecha: 03/12/2025
 * Archivo: point_set.cpp - Implementación de la clase point_set
 */

#include "point_set.hpp"

#include <cmath>
#include <iostream>

using namespace CyA;

/**
 * @brief Encuentra los puntos con las coordenadas x mínima y máxima.
 * @param min_x - punto con la coordenada x mínima
 * @param max_x - punto con la coordenada x máxima
 */
void point_set::x_bounds(point& min_x, point& max_x) const {
  min_x = max_x = (*this)[0];

  for (const point& p : *this) {
    if (p.first < min_x.first) min_x = p;
    if (p.first > max_x.first) max_x = p;
  }
}

/**
 * @brief Calcula la posición de un punto con respecto a una línea.
 * @param line - línea definida por dos puntos
 * @param p - punto a evaluar
 * @return valor positivo si el punto está a la izquierda de la línea,
 */
double point_set::point_2_line(const line& line, const point& point) const {
  return (point.second - line.first.second) * (line.second.first - line.first.first) -
         (line.second.second - line.first.second) * (point.first - line.first.first);
}

/**
 * @brief Determina de qué lado de la línea se encuentra el punto.
 * @param line - línea definida por dos puntos
 * @param point - punto a evaluar
 * @return LEFT, RIGHT o CENTER
 */
int point_set::find_side(const line& line, const point& point) const {
  double val = point_2_line(line, point);
  if (val > 0) return LEFT;
  if (val < 0) return RIGHT;
  return CENTER;
}

/**
 * @brief Calcula la distancia perpendicular desde un punto a una línea.
 * @param line - línea definida por dos puntos
 * @param point - punto desde el cual se mide la distancia
 * @return distancia perpendicular
 */
double point_set::distance(const line& line, const point& point) const {
  return std::abs(point_2_line(line, point));
}

/**
 * @brief Encuentra el punto más alejado de una línea en un lado específico.
 * @param line - línea definida por dos puntos
 * @param side - lado de la línea (LEFT o RIGHT)
 * @param farthest - punto más alejado encontrado
 * @return true si se encontró un punto, false en caso contrario
 */
bool point_set::farthest_point(const line& line , int side, point& farthest) const {
  double max_dist = -1.0;
  bool found = false;

  for (const point& point : *this) {
    if (find_side(line, point) == side) {
      double distance_value = distance(line, point);
      if (distance_value > max_dist) {
        max_dist = distance_value;
        farthest = point;
        found = true;
      }
    }
  }
  return found;
}
/**
 * @brief Algoritmo QuickHull recursivo.
 * @param lines - línea definida por dos puntos
 * @param side - lado de la línea (LEFT o RIGHT)
 */
void point_set::quickHull(const line& lines, int side) {
  point farthest;

  if (farthest_point(lines, side, farthest)) {
    quickHull(line(lines.first, farthest),
              -find_side(line(lines.first, farthest), lines.second));

    quickHull(line(farthest, lines.second),
              -find_side(line(farthest, lines.second), lines.first));
  } else {
    hull_.push_back(lines.first);
    hull_.push_back(lines.second);
  }
}

/**
 * @brief Ejecuta el algoritmo QuickHull para encontrar la envolvente convexa.
 */
void point_set::quickHull() {
  hull_.clear();

  if (size() < 2) return;

  point min_x, max_x;
  x_bounds(min_x, max_x);

  quickHull(line(min_x, max_x), LEFT);
  quickHull(line(min_x, max_x), RIGHT);

  std::sort(hull_.begin(), hull_.end());
  hull_.erase(std::unique(hull_.begin(), hull_.end()), hull_.end());
}

/**
 * @brief Escribe la envolvente convexa en un flujo de salida.
 * @param os - flujo de salida
 */
void point_set::write_hull(std::ostream& os) const {
  os << hull_.size() << "\n";
  for (const point& p : hull_) os << p.first << " " << p.second << "\n";
}

/**
 * @brief Escribe la representación en formato DOT del conjunto de puntos y su envolvente convexa.
 * @param os - flujo de salida
 */
void point_set::write(std::ostream& os) const {
  os << "graph{\n";

  for (size_t i = 0; i < hull_.size(); i++) {
    os << "  " << i << " [pos=\"" << hull_[i].first << "," << hull_[i].second
       << "!\"]\n";
  }

  for (size_t i = 0; i < hull_.size(); i++) {
    size_t j = (i + 1) % hull_.size();
    os << "  " << i << " -- " << j << "\n";
  }

  os << "}\n";
}
