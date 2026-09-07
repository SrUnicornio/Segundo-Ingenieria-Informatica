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
 * Archivo: point_set.hpp - Declaración de la clase point_set
 */

#ifndef POINT_SET_HPP__
#define POINT_SET_HPP__

#include <algorithm>
#include <vector>

#include "point_types.hpp"

namespace CyA {

enum side { LEFT = -1, CENTER, RIGHT };

class point_set : public point_vector {
 private:
  point_vector hull_;

 public:
  point_set(const point_vector& points) : point_vector(points), hull_() {}
  ~point_set() {}

  void quickHull();
  void write_hull(std::ostream& os) const;
  void write(std::ostream& os) const;

  inline const point_vector& get_hull() const { return hull_; }
  inline const point_vector& get_points() const { return *this; }

 private:
  void quickHull(const line& l, int side);
  double distance(const line& l, const point& p) const;
  int find_side(const line& l, const point& p) const;
  void x_bounds(point& min_x, point& max_x) const;
  double point_2_line(const line& l, const point& p) const;
  bool farthest_point(const line& l, int side, point& farthest) const;
};

}  // namespace CyA

#endif // POINT_SET_HPP__