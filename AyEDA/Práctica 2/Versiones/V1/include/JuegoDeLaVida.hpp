/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asigantura: Algoritmos y Estructuras de Datos Avanzadas
 *
 * @date: 03 FEB 2024
 * @author: Álvaro Pérez Ramos
 * @file: JuegoDeLaVida.hpp
 * @brief: Definition of differents Tools
 * */

#ifndef __JUEGODELAVIDA_HPP__
#define __JUEGODELAVIDA_HPP__

#include "Board.hpp"
#include "Cell.hpp"
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

struct program_options
{
  bool size_arg = false;
  bool init_arg = false;
  bool show_help = false;
  int filas;
  int columnas;
  std::string border;
  std::optional<State> v;
  std::string init;
};

void Usage(char *argv[]);
std::optional<program_options> parse_args(int argc, char *argv[]);
bool passe_size_arg(std::vector<std::string_view>::iterator &it, std::vector<std::string_view> &args, program_options &options);
bool passe_init_arg(std::vector<std::string_view>::iterator &it, std::vector<std::string_view> &args, program_options &options);
bool passe_border_arg(std::vector<std::string_view>::iterator &it, std::vector<std::string_view> &args, program_options &options);
bool process_option(std::string_view option, std::vector<std::string_view>::iterator &it, std::vector<std::string_view> &args, program_options &options, const std::unordered_map<std::string_view, std::function<bool(std::vector<std::string_view>::iterator &, std::vector<std::string_view> &, program_options &)>> &option_handlers);

Board InitBoard(const program_options &options);
bool UserWantsManualInput();
void SetLivingCellsManually(Cell **cell, const program_options &options);
bool IsValidPosition(int row, int column, const program_options &options);
void SetRandomLivingCells(Cell **cell, const program_options &options);


#endif // __JUEGODELAVIDA_HPP__
