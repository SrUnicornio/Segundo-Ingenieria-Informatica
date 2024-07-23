/**
 * @file Ordenacion_QuickSort.cc
 * 
 * compilar con: g++ -std=c++23 -o Ordenacion_QuickSort Ordenacion_QuickSort.cc
 */
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

template <typename Key>
void toLower(Key &value)
{
  // No hacemos nada por defecto, ya que no todos los tipos de datos son convertibles a minúsculas
}

template <>
void toLower(std::string &value)
{
  for (char &c : value)
  {
    if (std::isalpha(c))
    { // Solo convertimos caracteres alfabéticos
      c = std::tolower(c);
    }
  }
}

template <class Key, class Func>
void imprimirSecuencia(const Key *secuencia, int n, Func condicion)
{
  for (int i = 0; i < n; i++)
  {
    if (condicion(secuencia[i][0]))
    {
      std::cout << secuencia[i] << " ";
    }
  }
}

template <class Key>
int particion(Key *secuencia, int inicio, int fin)
{
  Key pivote = secuencia[(inicio + fin) / 2];

  while (inicio <= fin)
  {
    while (secuencia[inicio] < pivote)
    {
      inicio++;
    }
    while (secuencia[fin] > pivote)
    {
      fin--;
    }
    if (inicio <= fin)
    {
      std::swap(secuencia[inicio], secuencia[fin]);
      inicio++;
      fin--;
    }
  }
  return inicio;
}


template <class Key>
void Qsort( Key* secuencia, int inicio, int fin) {
  if (inicio < fin) {
    int p = particion(secuencia, inicio, fin);
    Qsort(secuencia, inicio, p - 1);
    Qsort(secuencia, p + 1, fin);
  }
}

int main()
{
  std::srand(std::time(nullptr));
  std::string secuencia[] = {"·#", "%", "@", "hola", "que", "tal", "estas", "bien", "gracias", "por", "preguntar", "90", "154", "a"};
  int n = sizeof(secuencia) / sizeof(secuencia[0]);

  std::cout << "¿Desea convertir la secuencia a minúsculas? (s/n): ";
  char respuesta;
  std::cin >> respuesta;

  if (respuesta == 's' || respuesta == 'S')
  {
    for (int i = 0; i < n; i++)
    {
      toLower(secuencia[i]);
    }
  }

  std::cout << "¿Cómo desea que sea el orden de la secuencia?\n"
            << "1. letras, números y otros caracteres\n"
            << "2. letras, otros caracteres y números\n"
            << "3. números, letras y otros caracteres\n"
            << "4. números, otros caracteres y letras\n"
            << "5. otros caracteres, letras y números\n"
            << "6. otros caracteres, números y letras\n"
            << "Introduzca el número de la opción: ";
  char orden;
  std::cin >> orden;

  auto start = std::chrono::high_resolution_clock::now();
  Qsort(secuencia, 0, n - 1);
  auto end = std::chrono::high_resolution_clock::now();

  std::cout << "Secuencia ordenada: ";

  // Mostramos la secuencia ordenada como quiere el usuario
  switch (orden)
  {
  case '1':
    imprimirSecuencia(secuencia, n, [](char c)
                      { return std::isalpha(c); });
    imprimirSecuencia(secuencia, n, [](char c)
                      { return std::isdigit(c); });
    imprimirSecuencia(secuencia, n, [](char c)
                      { return !std::isalpha(c) && !std::isdigit(c); });
    break;
  case '2':
    imprimirSecuencia(secuencia, n, [](char c)
                      { return std::isalpha(c); });
    imprimirSecuencia(secuencia, n, [](char c)
                      { return !std::isalpha(c) && !std::isdigit(c); });
    imprimirSecuencia(secuencia, n, [](char c)
                      { return std::isdigit(c); });
    break;
  case '3':
    imprimirSecuencia(secuencia, n, [](char c)
                      { return std::isdigit(c); });
    imprimirSecuencia(secuencia, n, [](char c)
                      { return std::isalpha(c); });
    imprimirSecuencia(secuencia, n, [](char c)
                      { return !std::isalpha(c) && !std::isdigit(c); });
    break;
  case '4':
    imprimirSecuencia(secuencia, n, [](char c)
                      { return std::isdigit(c); });
    imprimirSecuencia(secuencia, n, [](char c)
                      { return !std::isdigit(c) && std::isalpha(c); });
    imprimirSecuencia(secuencia, n, [](char c)
                      { return !std::isalpha(c); });
    break;
  case '5':
    imprimirSecuencia(secuencia, n, [](char c)
                      { return !std::isalpha(c) && !std::isdigit(c); });
    imprimirSecuencia(secuencia, n, [](char c)
                      { return std::isalpha(c); });
    imprimirSecuencia(secuencia, n, [](char c)
                      { return std::isdigit(c); });
    break;
  case '6':
    imprimirSecuencia(secuencia, n, [](char c)
                      { return !std::isdigit(c) && !std::isalpha(c); });
    imprimirSecuencia(secuencia, n, [](char c)
                      { return std::isdigit(c); });
    imprimirSecuencia(secuencia, n, [](char c)
                      { return std::isalpha(c); });
    break;
  default:
    std::cout << "Opción no válida" << std::endl;
    break;
  }
  std::cout << std::endl;

  std::chrono::duration<double, std::milli> duration = end - start;
  std::cout << "Tiempo: " << duration.count() << "ms" << std::endl;

  return 0;
}
