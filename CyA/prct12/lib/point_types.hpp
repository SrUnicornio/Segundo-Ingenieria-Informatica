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
 * Archivo: point_types.hpp - Declaración de tipos de datos para puntos y líneas
 */

#pragma once

#include <iostream>
#include <set>
#include <utility>
#include <vector>

#define MAX_SZ 3
#define MAX_PREC 0

namespace CyA {
typedef std::pair<double, double> point;

typedef std::pair<point, point> line;
typedef std::vector<point> point_vector;

typedef std::pair<point, point> arc;
typedef std::pair<double, arc> weigthed_arc;
typedef std::vector<weigthed_arc> arc_vector;

typedef std::set<point> point_collection;

typedef std::vector<arc> tree;
}  // namespace CyA

std::ostream& operator<<(std::ostream& os, const CyA::point_vector& ps);
std::ostream& operator<<(std::ostream& os, const CyA::point& ps);

std::istream& operator>>(std::istream& is, CyA::point_vector& ps);
std::istream& operator>>(std::istream& is, CyA::point& ps);