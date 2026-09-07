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
 * Archivo:z
 */
#include "point_set.hpp"

#include <algorithm>
#include <utility>

// destructor por defecto
point_set::~point_set(void) { emst_.clear(); }

/**
 * @brief método para calcular los costes entre cada par de puntos
 * @param av vector de arcos
 */

void point_set::compute_arc_vector(CyA::arc_vector &av) const {
  av.clear();

  const int n = size();

  for (int i = 0; i < n - 1; ++i) {
    const CyA::point &p_i = (*this)[i];

    for (int j = i + 1; j < n; ++j) {
      const CyA::point &p_j = (*this)[j];

      const double dist = euclidean_distance(std::make_pair(p_i, p_j));

      av.push_back(std::make_pair(dist, std::make_pair(p_i, p_j)));
    }
  }

  std::sort(av.begin(), av.end());
}

/**
 * @brief método para encontrar los subárboles incidentes
 * @param st bosque de subárboles
 * @param a arco
 * @param i índice del primer subárbol
 * @param j índice del segundo subárbol
 */
void point_set::find_incident_subtrees(const forest &st, const CyA::arc &a,
                                       int &i, int &j) const {
  i = j = -1;

  for (size_t index = 0; index < st.size(); ++index) {
    const EMST::sub_tree &subtree = st[index];
    if (subtree.contains(a.first)) {
      i = index;  // Found subtree containing the first point of the arc
    }
    if (subtree.contains(a.second)) {
      j = index;  // Found subtree containing the second point of the arc
    }

    // If both incident subtrees are found, break the loop
    if (i != -1 && j != -1) {
      break;
    }
  }
}

/**
 * @brief método para unir los subárboles incidentes
 * @param st bosque de subárboles
 * @param a arco
 * @param i índice del primer subárbol
 * @param j índice del segundo subárbol
 */
void point_set::merge_subtrees(forest &st, const CyA::arc &a, int i,
                               int j) const {
  const EMST::sub_tree &subtree_i = st[i];
  const EMST::sub_tree &subtree_j = st[j];

  const double cost = euclidean_distance(a);

  EMST::sub_tree subtree(subtree_i);
  subtree.merge(subtree_j, std::make_pair(cost, a));

  st[i] = subtree;
  st.erase(st.begin() + j);
}

/**
 * @brief método para calcular el coste total del árbol
 * @return coste total del árbol
 */
double point_set::compute_cost(void) const {
  double cost = 0;

  for (const CyA::arc &a : emst_) {
    cost += euclidean_distance(a);
  }

  return cost;
}

/**
 * @brief método para calcular la distancia euclídea entre dos puntos
 * @param a arco
 * @return distancia euclídea entre dos puntos
 */
double point_set::euclidean_distance(const CyA::arc &a) const {
  const double distancia_x = a.first.first - a.second.first;
  const double distancia_y = a.first.second - a.second.second;

  return sqrt(distancia_x * distancia_x + distancia_y * distancia_y);
}

/**
 * @brief método para calcular el árbol de expansión mínima
 */
void point_set::EMST(void) {
  forest bosque_F;
  for (size_t i = 0; i < size(); i++) {
    bosque_F.push_back(EMST::sub_tree((*this)[i]));
  }

  CyA::arc_vector aristas_S;
  compute_arc_vector(aristas_S);

  while (!aristas_S.empty() && bosque_F.size() != 1) {
    CyA::arc arista_minima = aristas_S[0].second;
    aristas_S.erase(aristas_S.begin());
    int i = 0;
    int j = 0;
    find_incident_subtrees(bosque_F, arista_minima, i, j);
    if (i != j && i != -1 && j != -1) {
      merge_subtrees(bosque_F, arista_minima, i, j);
    }
  }

  emst_ = bosque_F[0].get_arcs();
}

/**
 * @brief método para escribir el árbol de expansión mínima en formato texto
 * @param os flujo de salida
 */
void point_set::write_tree(std::ostream &os) const {
  for (const CyA::arc &a : emst_) {
    os << a.first << " -> " << a.second << std::endl;
  }
  os << compute_cost() << std::endl;
}

/**
 * @brief método para escribir el árbol de expansión mínima en formato dot
 * @param os flujo de salida
 */
void point_set::write(std::ostream &os) const {
  CyA::point_vector points = get_points();

  os << "graph{ " << std::endl << std::endl;

  for (size_t i = 0; i < size(); i++) {
    os << " " << i << " [pos = \"" << (*this)[i].first << ","
       << (*this)[i].second << "!\"]" << std::endl;
  }

  os << std::endl;

  // Imprimir posiciones de los puntos para cada arco
  for (const CyA::arc &a : emst_) {
    int i = 0;
    int j = 0;
    point_find(a.first, i);
    point_find(a.second, j);
    os << " " << i << " -- " << j << std::endl;
  }

  os << "}" << std::endl;
}

/**
 * @brief método para encontrar un punto en el conjunto de puntos
 * @param p punto
 * @param i índice del punto
 */
void point_set::point_find(const CyA::point &p, int &i) const {
  i = 0;
  for (const CyA::point &p_i : *this) {
    if (p_i == p) {
      return;
    }
    i++;
  }
  i = -1;
}