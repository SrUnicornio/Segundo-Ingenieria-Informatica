#ifndef __AUTOMATACELULARGENERAL_HPP__
#define __AUTOMATACELULARGENERAL_HPP__

#include "Lattice.hpp"
#include <vector>
#include <utility>
#include <string_view>
#include <unordered_map>
#include <functional>

struct program_options
{
  bool dimension_arg = false;
  bool size_arg = false;
  bool rows_cols_arg = false;
  bool init_arg = false;
  bool show_help = false;
  int dimension;
  int size;
  int rows;
  int cols;
  std::string cell_type;
  std::string border;
  std::optional<State> v;
  std::string init;
};


void Usage(char *argv[]);
std::optional<program_options> parse_args(const int argc, const char *const argv[]);

void handle_dimension(const program_options &options);
void handle_init(const program_options &options);

#endif // __AUTOMATACELULARGENERAL_HPP__