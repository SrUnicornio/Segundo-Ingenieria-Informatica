/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 12: Algoritmos divide y vencerás
 * Autor: 
 * Correo: alu0101574042@ull.edu.es
 * Fecha: 03/12/2025
 * Archivo: quickhull.hpp - Declaración de funciones para QuickHull
 */

#ifndef QUICKHULL_HPP__
#define QUICKHULL_HPP__

#include <fstream>
#include <iostream>
#include <string>

#include "point_types.hpp"

/// Opciones del programa (igual que en práctica 11)
struct ProgramOptions {
  std::string input_filename;
  std::string output_filename;
  std::string dot_filename;
  bool show_help = false;
};

void PrintUsage();
bool ParseArguments(int argc, char* argv[], ProgramOptions& options);

/// Lee puntos desde fichero
CyA::point_vector ReadPointsFromFile(const std::string& filename);

/// Escribe la envolvente convexa
void WriteHullToFile(const CyA::point_vector& hull,
                     const std::string& filename);

#endif // QUICKHULL_HPP__