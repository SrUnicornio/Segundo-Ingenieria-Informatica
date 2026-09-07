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
 * Archivo: point_set.hpp - Clase para representar un conjunto de puntos
 */

#ifndef POINT_SET_HPP__
#define POINT_SET_HPP__

#include <vector>

#include "point_types.hpp"
#include "sub_tree.hpp"

using namespace EMST;
typedef std::vector<EMST::sub_tree> forest;

class point_set : public CyA::point_vector {
 private:
  CyA::tree emst_;

 public:
  // point_set(const CyA::point_vector &points);
  point_set(const CyA::point_vector &points)
      : CyA::point_vector(points), emst_() {}

  ~point_set(void);

  void EMST(void);

  void write_tree(std::ostream &) const;
  void write(std::ostream &) const;

  inline const CyA::tree &get_tree(void) const { return emst_; }
  inline const CyA::point_vector &get_points(void) const { return *this; }
  inline double get_cost(void) const { return compute_cost(); }
  void point_find(const CyA::point &, int &) const;

 private:
  void compute_arc_vector(CyA::arc_vector &) const;
  void find_incident_subtrees(const forest &, const CyA::arc &, int &,
                              int &) const;
  void merge_subtrees(forest &, const CyA::arc &, int, int) const;

  double compute_cost(void) const;

  double euclidean_distance(const CyA::arc &) const;
};

#endif  // POINT_SET_HPP__