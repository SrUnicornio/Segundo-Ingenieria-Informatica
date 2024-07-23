#ifndef __ARBOL_BINARIO_H__
#define __ARBOL_BINARIO_H__

#include <iostream>
#include <queue>
#include <optional>
#include <fstream>
#include "ABB.h"
#include "ABE.h"
#include "NIF.h"

static bool firstTime = true;
struct program_options
{
  unsigned int nodeSize = 0;
  enum tree_tipe
  {
    ABE,
    ABB
  } tipe;
  std::string filename;
  bool isManual = false;
  bool isRandom = false;
  bool isFile = false;
};

/**
 * @brief Show the usage of the program
 */
void Usage()
{
  std::cout << "Usage: ./Albol_Binario\n";
  std::cout << "-ab <abe|abb> : to select the type of binary tree\n";
  std::cout << "-init <i> [s][f] : to select the initialization method\n";
  std::cout << "\t i = manual\n";
  std::cout << "\t i = random [s] : s is the number of nodes\n"; 
  std::cout << "\t i = file [s] [f] : s is the number of nodes and f is the file name\n";
}

/**
 * @brief Parse the arguments of the program
 * @param argc Number of arguments
 * @param argv Array of arguments
 * @return program_options struct with the options
 */
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
    if (*it == "-ab")
    {
      if (++it == args.end())
      {
        std::cerr << "Falta el tipo de árbol\n";
        return std::nullopt;
      }
      if (*it == "abe")
      {
        options.tipe = program_options::ABE;
      }
      else if (*it == "abb")
      {
        options.tipe = program_options::ABB;
      }
      else
      {
        std::cerr << "Tipo de árbol no válido\n";
        return std::nullopt;
      }
    }
    else if (*it == "-init")
    {
      if (++it == args.end())
      {
        std::cerr << "Falta el método de inicialización\n";
        return std::nullopt;
      }
      if (*it == "manual")
      {
        options.isManual = true;
      }
      else if (*it == "random")
      {
        options.isRandom = true;
        if (++it != args.end())
        {
          options.nodeSize = std::stoi(*it);
        }
      }
      else if (*it == "file")
      {
        options.isFile = true;
        if (++it != args.end())
        {
          options.nodeSize = std::stoi(*it);
        }
        if (++it != args.end())
        {
          options.filename = *it;
        }
      }
      else
      {
        std::cerr << "Método de inicialización no válido\n";
        return std::nullopt;
      }
    }
  }
  return options;
}

AB<NIF> *CreateTree(program_options::tree_tipe tipe)
{
  switch (tipe)
  {
  case program_options::ABE:
    return new ABE<NIF>();
    break;
  case program_options::ABB:
    return new ABB<NIF>();
    break;
  default:
    return nullptr;
    break;
  }
}

void selectOrder(AB<NIF> *ab)
{
  std::cout << "[0] InOrden\n"
               "[1] PreOrden\n"
               "[2] PostOrden\n"
               "[3] Niveles\n";

  std::cout << "Opción: ";
  int opcion;
  std::cin >> opcion;

  switch (opcion)
  {
  case 0:
    std::cout << "InOrden\n";
    ab->inOrder(ab->getRoot());
    break;
  case 1:
    std::cout << "PreOrden\n";
    ab->preOrder(ab->getRoot());
    break;
  case 2:
    std::cout << "PostOrden\n";
    ab->postOrder(ab->getRoot());
    break;
  case 3:
    std::cout << "Niveles\n";
    ab->levels(ab->getRoot());
    break;
  default:
    std::cout << "Opción no válida\n";
    break;
  }
  std::cout << std::endl;
}

void arbolTipe(AB<NIF> **ab)
{
  program_options::tree_tipe tipe;
  std::cout << "[0] ABE\n"
               "[1] ABB\n";

  std::cout << "Opción: ";
  int opcion;
  std::cin >> opcion;

  switch (opcion)
  {
  case 0:
    tipe = program_options::ABE;
    break;
  case 1:
    tipe = program_options::ABB;
    break;
  default:
    std::cout << "Opción no válida\n";
    return;
  }

  AB<NIF> *newTree = CreateTree(tipe); // Crear un nuevo árbol con el tipo seleccionado

  // Copiar los valores del árbol original al nuevo árbol
  if (*ab)
  {
    // Crear una cola para realizar un recorrido BFS
    std::queue<NodoB<NIF> *> q;
    q.push((*ab)->getRoot());

    while (!q.empty())
    {
      NodoB<NIF> *node = q.front();
      q.pop();

      // Insertar el nodo en el nuevo árbol
      newTree->insert(node->getData());

      // Agregar los hijos del nodo a la cola
      if (node->getleft())
        q.push(node->getleft());
      if (node->getright())
        q.push(node->getright());
    }
  }

  delete *ab;    // Eliminar el árbol anterior
  *ab = newTree; // Asignar el nuevo árbol al puntero original
}

void menu1(AB<NIF> *ab, program_options options)
{
  NIF nif;
  int opcion;
  do
  {
    std::cout << "[0] Salir\n"
                 "[1] Insertar clave\n"
                 "[2] Buscar clave\n"
                 "[3] Mostrar árbol\n"
                 "[4] Cambiar el tipo de árbol\n";

    std::cout << "Opción: ";
    std::cin >> opcion;
    switch (opcion)
    {
    case 0:
      std::cout << "Saliendo...\n";
      break;
    case 1:
      std::cout << "Insertar clave\n";
      if (options.isManual)
      {
        std::cout << "Introduce el NIF: ";

        std::cin >> nif;
        ab->insert(nif);
      }
      else if (options.isRandom)
      {
        if (firstTime)
        {
          for (int i = 0; i < options.nodeSize; i++)
          {
            nif = NIF();
            ab->insert(nif);
          }
          firstTime = false;
        }
        else
        {
          std::cout << "Introduce el NIF: ";
          std::cin >> nif;
          ab->insert(nif);
        }
      }
      else if (options.isFile)
      {
        if (firstTime)
        {
          std::ifstream file(options.filename);
          if (!file)
          {
            std::cerr << "Error al abrir el archivo\n";
            return;
          }

          while (file >> nif)
          {
            ab->insert(nif);
          }
          firstTime = false;
        }
        else
        {
          std::cout << "Introduce el NIF: ";
          std::cin >> nif;
          ab->insert(nif);
        }
      }
      break;
    case 2:
      std::cout << "Buscar clave\n";
      std::cout << "Introduce el NIF: ";
      std::cin >> nif;
      if (ab->search(nif))
      {
        std::cout << "Clave encontrada\n";
      }
      else
      {
        std::cout << "Clave no encontrada\n";
      }
      break;
    case 3:
      std::cout << "Mostrar árbol\n";
      selectOrder(ab);
      break;
    case 4:
      std::cout << "Cambiar el tipo de árbol\n";
      arbolTipe(&ab);
      break;
    default:
      std::cout << "Opción no válida\n";
      break;
    }

    // limpiar buffer
    std::cin.clear();

  } while (opcion != 0);
}

#endif