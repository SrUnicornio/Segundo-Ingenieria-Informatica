#ifndef __TOOLS_H__
#define __TOOLS_H__

#include "../includespath.h"
#include "../HashTable/HasTable.h"

struct program_options
{
  unsigned tableSize;
  std::string dispersionFunction;
  std::string hashType;
  unsigned blockSize;
  std::string explorationFunction;
};

void Usage()
{
  std::cout << "Usage: ./HashTable -ts <s> -fd <f> -hash <open|close> -bs <s> -fe <f>\n";
  std::cout << "-ts <s> : table size\n";
  std::cout << "-fd <f> : dispersion function -> sum, random, or mod\n";
  std::cout << "-hash <open|close> : collision resolution technique\n";
  std::cout << "-bs <s> : block size for closed addressing\n";
  std::cout << "-fe <f> : exploration function for closed addressing -> linear, quadratic, double, or rehashing\n";
}

std::optional<program_options> parse_args(int argc, const char *const argv[])
{
  program_options options;
  std::vector<std::string> args(argv + 1, argv + argc);
  if (args.empty())
  {
    return std::nullopt;
  }

  for (auto it = args.begin(); it != args.end(); ++it)
  {
    if (*it == "-ts")
    {
      if (it + 1 >= args.end())
      {
        return std::nullopt;
      }
      options.tableSize = std::stoi(*(it + 1));
      it += 1;
    }
    else if (*it == "-fd")
    {
      if (it + 1 >= args.end())
      {
        return std::nullopt;
      }
      options.dispersionFunction = *(it + 1);
      it += 1;
    }
    else if (*it == "-hash")
    {
      if (it + 1 >= args.end())
      {
        return std::nullopt;
      }
      options.hashType = *(it + 1);
      it += 1;
    }
    else if (*it == "-bs")
    {
      if (it + 1 >= args.end())
      {
        return std::nullopt;
      }
      options.blockSize = std::stoi(*(it + 1));
      it += 1;
    }
    else if (*it == "-fe")
    {
      if (it + 1 >= args.end())
      {
        return std::nullopt;
      }
      options.explorationFunction = *(it + 1);
      it += 1;
    }
  }

  return options;
}

template <class Key, class Container>
void Menu(HashTable<Key, Container> *table)
{
  std::cout << "1. Insertar un valor en la tabla\n";
  std::cout << "2. Buscar un valor en la tabla\n";
  std::cout << "3. Salir\n";

  std::cout << "Introduce una opción: ";

  int option;
  while (std::cin >> option)
  {
    switch (option)
    {
    case 1:
    {
      std::cout << "Introduce el Libro a insertar: ";
      Libro value;
      std::cin >> value;
      std::cout << "Insert in table: " << (table->insert(value) ? "Success" : "Failed") << std::endl;
      break;
    }
    case 2:
    {
      std::cout << "Introduce el Libro a buscar: ";
      Libro value;
      std::cin >> value;
      std::cout << "Search in table: " << (table->search(value) ? "Found" : "Not found") << std::endl;
      std::cout << "Value of Libro: " << value << " in position: " << table->GetIndex(value) << std::endl;
      break;
    }
    case 3:
      std::cout << "Saliendo...\n";
      return;
    default:
      std::cout << "Opción no válida\n";
      break;
    }
    std::cout << "1. Insertar un valor en la tabla\n";
    std::cout << "2. Buscar un valor en la tabla\n";
    std::cout << "3. Salir\n";
    std::cout << "Introduce una opción: ";
  }
}

void initializeFunctions(const program_options &options, DispersionFunction<Libro> *&dispersion, ExplorationFunction<Libro> *&exploration)
{
  if (options.dispersionFunction == "sum")
  {
    dispersion = new SumDispersion<Libro>(options.tableSize);
  }
  else if (options.dispersionFunction == "random")
  {
    dispersion = new PseudoRandomDispersion<Libro>(options.tableSize);
  }
  else if (options.dispersionFunction == "mod")
  {
    dispersion = new ModuloDispersion<Libro>(options.tableSize);
  }
  else
  {
    Usage();
    exit(1);
  }

  if (options.explorationFunction == "linear")
  {
    exploration = new LinearExploration<Libro>;
  }
  else if (options.explorationFunction == "quadratic")
  {
    exploration = new QuadraticExploration<Libro>;
  }
  else if (options.explorationFunction == "double")
  {
    exploration = new DoubleHashExploration<Libro>(dispersion);
  }
  else if (options.explorationFunction == "rehashing")
  {
    exploration = new RehashingExploration<Libro>(dispersion);
  }
  else
  {
    Usage();
    exit(1);
  }
}

#endif // __TOOLS_H__
