/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asigantura: Algoritmos y Estructuras de Datos Avanzadas
 *
 * @date: 12 FEB 2024
 * @author: 
 * @file: JuegoDeLaVida_main.cpp
 * @brief Implemtation of game od live
 * */
#include "../include/JuegoDeLaVida.hpp"

int main(int argc, char *argv[])
{
  std::optional<program_options> options = parse_args(argc, argv);
  if (!options || options->show_help || (!options->size_arg && !options->init_arg))
  {
    std::cerr << "Invalid arguments." << std::endl;
    Usage(argv);
    return 1;
  }

  if (options->size_arg)
  {
    Board board = InitBoard(*options);
    board.RunGame();
  }
  else if (options->init_arg)
  {
    std::cout << "Loading initial configuration from file " << options->init << std::endl;
    Board board(options->init.c_str(), options->border, options->v);
    board.RunGame();
  }

  return 0;
}