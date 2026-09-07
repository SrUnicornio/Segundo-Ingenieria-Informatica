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
 * Archivo: sub_tree.hpp - Declaración de la clase sub_tree
 */

#ifndef SUB_TREE_HPP__
#define SUB_TREE_HPP__

#include <cmath>
#include <iostream>

#include "point_types.hpp"

namespace EMST {
class sub_tree {
 public:
  sub_tree() : cost_(0) {}
  sub_tree(const CyA::point& point) : arcs_(), points_({point}), cost_(0) {}
  sub_tree(const sub_tree& other)
      : arcs_(other.arcs_), points_(other.points_), cost_(other.cost_) {}
  ~sub_tree() {}

  void add_arc(const CyA::arc&);
  void add_point(const CyA::point&);
  bool contains(const CyA::point&) const;
  void merge(const sub_tree&, const CyA::weigthed_arc&);

  inline const CyA::tree& get_arcs() const { return arcs_; }
  inline double get_cost() const { return cost_; }
  
  sub_tree& operator=(const sub_tree&);

 private:
  CyA::tree arcs_;
  CyA::point_collection points_;
  double cost_;
};
typedef std::vector<sub_tree> sub_tree_vector;
}  // namespace EMST

#endif  // SUB_TREE_HPP__