#include "AutomataCelulargeneral.hpp"
#include <thread>

/**
 * @brief Parse the command line arguments.
 * @param argc Number of arguments..
 * @param argv Arguments.
 * @return Options of the command line.
 */
std::optional<program_options> parse_args(const int argc, const char *const argv[])
{
  program_options options;
  std::vector<std::string_view> args(argv + 1, argv + argc);
  if (args.empty())
  {
    return std::nullopt;
  }

  for (auto it = args.begin(); it != args.end(); ++it)
  {
    if (*it == "-dim")
    {
      if (it + 1 >= args.end())
      {
        return std::nullopt;
      }
      if (std::stoi(std::string(*(it + 1))) != 1 && std::stoi(std::string(*(it + 1))) != 2)
      {
        return std::nullopt;
      }
      options.dimension = std::stoi(std::string(*(it + 1)));
      options.dimension_arg = true;
      it += 1;
    }
    else if (*it == "-size")
    {
      std::cout << options.dimension << std::endl;
      if (options.dimension == 1)
      {
        if (it + 1 >= args.end())
        {
          return std::nullopt;
        }
        options.size = std::stoi(std::string(*(it + 1)));
        options.size_arg = true;
        it += 1;
      }
      else if (options.dimension == 2)
      {
        if (it + 2 >= args.end())
        {
          return std::nullopt;
        }
        options.rows = std::stoi(std::string(*(it + 1)));
        options.cols = std::stoi(std::string(*(it + 2)));
        options.rows_cols_arg = true;
        it += 2;
      }
    }
    else if (*it == "-init")
    {
      if (it + 1 >= args.end())
      {
        return std::nullopt;
      }
      options.init = std::string(*(it + 1));
      it += 1;
      options.init_arg = true;
    }
    else if (*it == "-cell")
    {
      if (it + 1 >= args.end())
      {
        return std::nullopt;
      }
      options.cell_type = std::string(*(it + 1));
      it += 1;
    }
    else if (*it == "-border")
    {
      if (it + 1 >= args.end())
      {
        return std::nullopt;
      }
      options.border = std::string(*(it + 1));
      it += 1;
      if (options.border == "open")
      {
        if (it + 1 >= args.end())
        {
          return std::nullopt;
        }
        options.v = std::stoi(std::string(*(it + 1)));
        it += 1;
      }
      else if (options.border == "noborder" || options.border == "periodic" || options.border == "reflective")
      {
        options.v = kDead;
      }
    }
    else if (*it == "-h")
    {
      options.show_help = true;
    }
    else
    {
      return std::nullopt;
    }
  }

  return options;
}

/**
 * @brief Show the usage of the program.
 * @param argc number of arguments.
 * @param argv arguments.
 */
void Usage(char *argv[])
{
  std::cout << "Usage: " << argv[0] << " [-dim <d> -size <N> [<M>]] | [-init <file>] -cell <t> -border <b> [v]\n";
  std::cout << "Options:\n";
  std::cout << "  -dim <d>           Dimension of the automaton (default: 2)\n";
  std::cout << "  -size <N> [<M>]   Size of the lattice. If dim is 2, N is the number of rows and M is the number of columns (default: 10)\n";
  std::cout << "  -init <file>       Name of the file with the initial configuration\n";
  std::cout << "  -cell <t>          Type of cell automaton: ACE30, ACE110, LIFE51_346, LIVE23_3\n";
  std::cout << "  -border <b>        Type of lattice border: reflective, periodic, open, noborder\n";
  std::cout << "                     If b=open, the optional parameter v must be given (0 or 1) for cold or hot\n";
  std::cout << "Notes:\n";
  std::cout << "  -size and -init are mutually exclusive\n";

  exit(EXIT_FAILURE);
}

// un mapa de ACE30, ACE110, LIFE51_346, LIFE23_3 para los constructores de factori cell correspondientes
std::unordered_map<std::string, std::function<FactoryCell *()>> factory_map = {
    {"ACE30", []() { return new FactoryCellACE30(); }},
    {"ACE110", []() { return new FactoryCellACE110(); }},
    {"LIFE51_346", []() { return new FactoryCellLife51_346(); }},
    {"LIFE23_3", []() { return new FactoryCellLife23_3(); }}};


/**
 * @brief Handle the dimension of the automaton.
 * @param options Options of the command line.
 */
void handle_dimension(const program_options &options)
{
  if (options.dimension == 1)
  {
    if (options.size_arg)
    {
      // usa el mapa para crear la factori cell correspondiente
      FactoryCell *factory = factory_map[options.cell_type]();
      PositionDim<1> position1(1, options.size);
      if (options.border == "open")
      {
        Lattice1D_open lattice1(position1, *factory, options.border, options.v.value_or(kDead));
        // Poner las celulas en el estado inicial de forma aleatoria o manual
        char answer;
        std::cout << "Do you want to set the cells manually? (y/n): ";
        std::cin >> answer;
        answer = tolower(answer);
        lattice1.RunGame();
      }
      else
      {
        Lattice1D_periodic lattice1(position1, *factory, options.border, kDead);
        lattice1.RunGame();
      }
    }
  }
  else if (options.dimension == 2)
  {
    if (options.rows_cols_arg)
    {
      // usa el mapa para crear la factori cell correspondiente
      FactoryCell *factory = factory_map[options.cell_type]();
      PositionDim<2> position2(2, options.rows, options.cols);
      if (options.border == "noborder")
      {
        Lattice2D_noborder lattice2(position2, *factory, options.border);
        lattice2.RunGame();
      }
      else
      {
        Lattice2D_reflective lattice2(position2, *factory, options.border);
        lattice2.RunGame();
      }
    }
  }
}

void handle_init(const program_options &options)
{
  // usa el mapa para crear la factori cell correspondiente
  FactoryCell *factory = factory_map[options.cell_type]();
  if (options.border == "noborder")
  {
    Lattice2D_noborder lattice2(options.init.c_str(), *factory, options.border, options.v.value_or(kDead));
    lattice2.RunGame();
  }
  else if (options.border == "reflective")
  {
    Lattice2D_reflective lattice2(options.init.c_str(), *factory, options.border, options.v.value_or(kDead));
    lattice2.RunGame();
  }
  else if (options.border == "periodic")
  {
    Lattice1D_periodic lattice2(options.init.c_str(), *factory, options.border, options.v.value_or(kDead));
    lattice2.RunGame();
  }
  else if (options.border == "open")
  {
    Lattice1D_open lattice2(options.init.c_str(), *factory, options.border, options.v.value_or(kDead));
    lattice2.RunGame();
  }
}
