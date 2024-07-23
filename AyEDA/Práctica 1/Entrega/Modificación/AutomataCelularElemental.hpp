#include "Cell.hpp"
#include "Lattice.hpp"

#ifndef __AUTOMATA_CELULAR_ELEMENTAL_HPP__
#define __AUTOMATA_CELULAR_ELEMENTAL_HPP__

struct program_options
{
  bool show_help = false;
  int size = 0;
  std::string border;
  std::optional<State> v; // Add the namespace prefix to the State type
  std::string init;
};

std::optional<program_options> parse_args(int argc, char *argv[]);
void Usage(char *argv[]);

#endif // __AUTOMATA_CELULAR_ELEMENTAL_HPP__