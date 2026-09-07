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
 * Archivo: sub_tree.cpp - Implementación de la clase sub_tree
 */
#include "sub_tree.hpp"

/**
 * @brief Añade un arco al subárbol
 * @param arc arco a añadir
 */
void EMST::sub_tree::add_arc(const CyA::arc& arc) {
  arcs_.push_back(arc);
  points_.insert(arc.first);
  points_.insert(arc.second);
}

/**
 * @brief Añade un punto al subárbol
 * @param points punto a añadir
 */
void EMST::sub_tree::add_point(const CyA::point& points) { points_.insert(points); }

bool EMST::sub_tree::contains(const CyA::point& points) const {
  return points_.find(points) != points_.end();
}

/**
 * @brief Fusiona dos subárboles
 * @param sub_trees subárbol a fusionar
 * @param arc arco que une los dos subárboles
 */
void EMST::sub_tree::merge(const EMST::sub_tree& sub_trees,
                           const CyA::weigthed_arc& arc) {
  arcs_.insert(arcs_.end(), sub_trees.arcs_.begin(), sub_trees.arcs_.end());
  arcs_.push_back(arc.second);

  points_.insert(sub_trees.points_.begin(), sub_trees.points_.end());
  cost_ += arc.first + sub_trees.get_cost();
}

/**
 * @brief Sobrecarga del operador de asignación
 * @param other subárbol a asignar
 */
EMST::sub_tree& EMST::sub_tree::operator=(const EMST::sub_tree& other) {
  if (this != &other) {  // Protect against self-assignment
    arcs_ = other.arcs_;
    points_ = other.points_;
    cost_ = other.cost_;
  }
  return *this;
}