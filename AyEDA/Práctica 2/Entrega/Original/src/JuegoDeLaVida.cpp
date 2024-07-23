#include "../include/JuegoDeLaVida.hpp"

/**
 * @brief Parse the size argument.
 * @param argc Number of arguments.
 * @param argv Arguments.
 * @return Options of the command line.
 */
bool passe_size_arg(std::vector<std::string_view>::iterator &it, std::vector<std::string_view> &args, program_options &options)
{
  options.size_arg = true;
  if (it + 2 >= args.end())
  {
    return false;
  }
  options.filas = std::stoi(std::string(*(it + 1)));
  options.columnas = std::stoi(std::string(*(it + 2)));
  it += 2;
  return true;
}

/**
 * @brief Parse the init argument.
 * @param argc Number of arguments.
 * @param argv Arguments.
 * @return Options of the command line.
 */
bool passe_init_arg(std::vector<std::string_view>::iterator &it, std::vector<std::string_view> &args, program_options &options)
{
  options.init_arg = true;
  if (it + 1 >= args.end())
  {
    return false;
  }
  options.init = std::string(*(it + 1));
  it += 1;
  return true;
}

/**
 * @brief Parse the border argument.
 * @param argc Number of arguments.
 * @param argv Arguments.
 * @return Options of the command line.
 */
bool passe_border_arg(std::vector<std::string_view>::iterator &it, std::vector<std::string_view> &args, program_options &options)
{
  if (it + 1 >= args.end())
  {
    return false;
  }
  options.border = std::string(*(it + 1));
  it += 1;
  if (options.border == "open")
  {
    if (it + 1 >= args.end())
    {
      return false;
    }
    options.v = std::stoi(std::string(*(it + 1)));
    it += 1;
  }
  else if (options.border == "noborder")
  {
    options.v = kDead;
  }
  else
  {
    options.v = std::nullopt;
  }
  return true;
}

/**
 * @brief Parse the help argument.
 * @param argc Number of arguments.
 * @param argv Arguments.
 * @return Options of the command line.
 */
bool passe_help_arg(std::vector<std::string_view>::iterator &it, std::vector<std::string_view> &args, program_options &options)
{
  options.show_help = true;
  return true;
}

/**
 * @brief Process the option.
 * @param option 
 * @param it 
 * @param args 
 * @param options 
 * @param option_handlers 
 * @return 
 */
bool process_option(std::string_view option, std::vector<std::string_view>::iterator &it, std::vector<std::string_view> &args, program_options &options, const std::unordered_map<std::string_view, std::function<bool(std::vector<std::string_view>::iterator &, std::vector<std::string_view> &, program_options &)>> &option_handlers)
{
  auto handler = option_handlers.find(option);
  if (handler == option_handlers.end())
  {
    std::cerr << "Error: Unrecognized option '" << option << "'." << std::endl;
    return false;
  }

  if (!handler->second(it, args, options))
  {
    std::cerr << "Error: Failed to process option '" << option << "'." << std::endl;
    return false;
  }

  if (options.size_arg && options.init_arg)
  {
    std::cerr << "Error: Cannot specify both size and init options." << std::endl;
    return false;
  }

  return true;
}

/**
 * @brief Parsea los argumentos de la línea de comandos.
 * @param argc Número de argumentos.
 * @param argv Argumentos.
 * @return Opciones de la línea de comandos.
 * @note Si se pasa un argumento -h o --help, se muestra la ayuda y se devuelve un objeto con el campo show_help a true.
 */
std::optional<program_options> parse_args(int argc, char *argv[])
{
  std::vector<std::string_view> args(argv + 1, argv + argc);
  program_options options;

  if (args.empty())
  {
    return std::nullopt;
  }

  std::unordered_map<std::string_view, std::function<bool(std::vector<std::string_view>::iterator &, std::vector<std::string_view> &, program_options &)>> option_handlers = {
      {"-size", passe_size_arg},
      {"-init", passe_init_arg},
      {"-border", passe_border_arg},
      {"-h", passe_help_arg},
      {"--help", passe_help_arg}
  };
  auto it = args.begin();
  while (it != args.end())
  {
    if (!process_option(*it, it, args, options, option_handlers))
    {
      return std::nullopt;
    }
    it++;
  }

  return options;
}

/**
 * @brief Muestra el uso del programa.
 * @param argc Número de argumentos.
 * @param argv Argumentos.
 */
void Usage(char *argv[])
{
  std::cout << "Use: " << argv[0] << " (-size <M> <N> | [-init <file>]) -border <b> [<v>]\n";
  std::cout << "M y N are the number of rows and columns of the lattice, respectively.\n";
  std::cout << "b=reflective | periodic | open | noborder, if b=open, the optional parameter v must be given (0 or 1) cold or hot.\n";
  std::cout << "file is the name of the file with the initial configuration.\n";
  std::cout << "-size and -init are mutually exclusive.\n";
  exit(EXIT_FAILURE);
}

/**
 * @brief Initialize the board manually or randomly.
 * @param options 
 * @return 
 */
Board InitBoard(const program_options &options)
{
  Cell **cells = new Cell *[options.filas];
  for (int i = 0; i < options.filas; i++)
  {
    cells[i] = new Cell[options.columnas];
  }

  if (UserWantsManualInput())
  {
    SetLivingCellsManually(cells, options);
  }
  else
  {
    SetRandomLivingCells(cells, options);
  }

  return Board(cells, options.filas, options.columnas, options.border, options.v);
}

/**
 * @brief Ask the user if he wants to introduce the position of the living cells.
 * @return True if the user wants to introduce the position of the living cells, false otherwise.
 */
bool UserWantsManualInput()
{
  std::cout << "Do you want to introduce the position of the living cells? (y/n): ";
  char answer;
  std::cin >> answer;
  return std::tolower(answer) == 'y';
}

/**
 * @brief Set the living cells manually.
 * @param cells 
 * @param options get rows and columns.
 */
void SetLivingCellsManually(Cell **cells, const program_options &options)
{
  int row, column;
  std::cout << "Enter the position of the living cells (row column): ";
  std::cout << "Enter 0 0 to finish." << std::endl;

  while (std::cin >> row >> column)
  {
    if (row == 0 && column == 0)
    {
      break;
    }
    if (IsValidPosition(row-1, column-1, options))
    {
      cells[row-1][column-1].SetState(kAlive);
    }
    else
    {
      std::cerr << "Invalid position. Please try again." << std::endl;
    }
    std::cout << "Enter the position of the living cells (row column): ";
    std::cout << "Enter 0 0 to finish." << std::endl;
  }
}

/**
 * @brief Check if the position is valid.
 * @param row 
 * @param column 
 * @param options 
 * @return 
 */
bool IsValidPosition(int row, int column, const program_options &options)
{
  return row >= 0 && row < options.filas && column >= 0 && column < options.columnas;
}

/**
 * @brief Set the living cells randomly.
 * @param cells 
 * @param options get rows and columns.
 */
void SetRandomLivingCells(Cell **cells, const program_options &options)
{
  std::cout << "Randomly generating the position of the living cells." << std::endl;
  srand(time(NULL));
  for (int i = 0; i < options.filas; i++)
  {
    for (int j = 0; j < options.columnas; j++)
    {
      cells[i][j].SetState(rand() % 2);
    }
  }
}