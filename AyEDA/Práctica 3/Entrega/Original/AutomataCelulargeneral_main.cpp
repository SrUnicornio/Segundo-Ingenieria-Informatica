#include "AutomataCelulargeneral.hpp"

int main(int argc, char *argv[])
{
  std::optional<program_options> options = parse_args(argc, argv);
  if (!options || options->show_help)
  {
    Usage(argv);
    return 1;
  }

  srand(time(NULL));
  if (options->dimension_arg)
  {
    handle_dimension(*options);
  }
  else if (options->init_arg)
  {
    handle_init(*options);
  }
  return 0;
}
