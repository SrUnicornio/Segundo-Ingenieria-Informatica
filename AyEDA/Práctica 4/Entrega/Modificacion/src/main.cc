#include "../Libro/Libro.h"
#include "../tools/tools.h"

int main(int argc, char const *argv[])
{
  // Pruebas();
  auto options = parse_args(argc, argv);
  if (!options)
  {
    Usage();
    return 1;
  }

  DispersionFunction<Libro> *dispersion;
  ExplorationFunction<Libro> *exploration;

  initializeFunctions(*options, dispersion, exploration);
  
  unsigned tableSize = options->tableSize;
  unsigned blockSize = options->blockSize;
  if (options->hashType == "open")
  {
    HashTable<Libro, dynamicSequence<Libro>> table(tableSize, *dispersion);
    Menu(&table);
  }
  else if (options->hashType == "close")
  {
    HashTable<Libro, staticSequence<Libro>> table(tableSize, *dispersion, *exploration, blockSize);
    Menu(&table);
  }
  else
  {
    Usage();
    return 1;
  }

  std::cout << "Number of compare and swap operations: " << compare_swapkeys_count << std::endl;
  return 0;
}
