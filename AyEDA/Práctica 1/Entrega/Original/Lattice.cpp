/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asigantura: Algoritmos y Estructuras de Datos Avanzadas
 *
 * @date: 03 FEB 2024
 * @author: Álvaro Pérez Ramos
 * @file Lattice.cpp
 * @brief Implementación de la clase Lattice
 */

#include "Lattice.hpp"

/**
 * @brief Constructor of the lattice
 * @param data Array of cells
 * @param size Size of the lattice
 * @param border Border of the lattice
 * @param v Optional state
 * @return void
 */
Lattice::Lattice(Cell cell[], int size, const std::string &border, const std::optional<State> v)
{
  size_ = size;
  border_ = border;
  v_ = v;
  cell_ = NULL;
  cell_ = new Cell[size];
  for (int i = 0; i < size; i++)
  {
    cell_[i] = cell[i];
  }
}

/**
 * @brief Load the initial state of the lattice from a file
 * @param filename
 * @return void
 */

void Lattice::Load(const std::string &filename)
{
  std::ifstream input(filename);
  if (!input.is_open())
  {
    std::cerr << "Error al abrir el fichero " << filename << std::endl;
    exit(EXIT_FAILURE);
  }
  std::string line;
  std::getline(input, line);
  if (line.size() != size_)
  {
    std::cout << "The size of the lattice does not match the size of the file." << std::endl;
    std::cout << "The lattice will be comleted with zeros or truncated." << std::endl;
    std::cout << "Do you want to continue? (y/n) ";
    char answer;
    std::cin >> answer;
    if (answer == 'n' || answer == 'N')
    {
      return;
    }
  }
  for (int i = 0; i < size_; i++)
  {
    cell_[i].SetState(line[i] - '0');
  }
  std::cout << "Configuración inicial del autómata celular: " << line << std::endl;
  input.close();
}

/**
 * @brief Calculates the next state of the cell
 */

void Lattice::NextGeneration()
{
  for (int i = 0; i < size_; i++)
  {
    cell_[i].NextState(*this, i);
  }
  for (int i = 0; i < size_; i++)
  {
    cell_[i].UpdateState();
  }
}

/**
 * @brief Overload of the insertion operator in flow
 * The lattice is responsible for its display on the screen, which it does by overloading the insertion operator in flow. To facilitate the display, the character 'X' will be used to represent the state value "1", and the space character ' ' for the state value "0".
 * @param os
 * @param lattice
 * @return States of the cells
 */

std::ostream &operator<<(std::ostream &os, const Lattice &lattice)
{
  for (int i = 0; i < lattice.size_; i++)
  {
    os << lattice.cell_[i];
  }
  return os;
}
