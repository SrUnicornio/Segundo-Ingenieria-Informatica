#include "AutomataCelularElemental.hpp"
#include <vector>

/**
 * @brief Parsea un string a un entero.
 * @param str String a parsear.
 * @param h Hash.
 * @return Entero.
 */
constexpr unsigned int str2int(const char *str, int h = 0)
{
  return !str[h] ? 5381 : (str2int(str, h + 1) * 33) ^ str[h];
}

/**
 * @brief Parsea los argumentos de la linea de comandos.
 * @param argc Numero de argumentos.
 * @param argv Argumentos.
 * @return Opciones de la linea de comandos.
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
  for (auto it = args.begin(), end = args.end(); it != end; ++it)
  {
    switch (str2int(it->data()))
    {
    case str2int("-s"):
    case str2int("-size"):
      if (++it != end)
      {
        options.size = std::stoi(std::string(*it));
        if (options.size < 0)
        {
          std::cerr << "El tamaño del retículo debe ser un número positivo." << std::endl;
          return std::nullopt;
        }
      }
      break;
    case str2int("-b"):
    case str2int("-border"):
      if (++it != end)
      {
        options.border = *it;
        if (++it != end) 
        {
          switch (str2int(options.border.data()))
          {
          case str2int("o"):
          case str2int("open"):
            if (*it != "0" && *it != "1")
            {
              std::cerr << "El valor de la frontera abierta debe ser 0 o 1." << std::endl;
              return std::nullopt;
            }
            options.v = *it == "0" ? 0 : 1;
            break;
          case str2int("p"):
          case str2int("periodic"):
          case str2int("r"):
          case str2int("reflected"):
            options.v = 0;
            --it;
            break;
          default:
            std::cerr << "El tipo de frontera debe ser 'open' (fria caliente), 'periodic' or 'reflected." << std::endl;
            return std::nullopt;
            break;
          }
        }
      }
      break;
    case str2int("-i"):
    case str2int("-init"):
      if (++it != end)
      {
        options.init = *it;
        if (options.init.empty())
        {
          std::cerr << "El nombre del fichero de configuración inicial no puede estar vacío." << std::endl;
          return std::nullopt;
        }
      }
      break;
    case str2int("-h"):
    case str2int("--help"):
      options.show_help = true;
      break;
    default:
      std::cerr << "Opción no reconocida: " << *it << std::endl;
      return std::nullopt;
      break;
    }
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
  std::cout << "Uso: " << argv[0] << " -size|-s <n> -border|-b <b [v]> [-init|-i <file>] \n";
  std::cout << "n es el tamaño del retículo. Número de células. \n";
  std::cout << "b=open b=periodic or b=reflected, v=[0|1]. Frontera abierta, fría o caliente. (only for open border)\n";
  std::cout << "file es un nombre del fichero que contiene un array de estados con la configuración inicial del autómata celular.\n";
  exit(EXIT_FAILURE);
}