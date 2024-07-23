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

template <class Key>
void Selection(Key *secuencia, int n)
{
  for (int i = 0; i < n - 1; i++)
  {
    int min = i;
    for (int j = i + 1; j < n; j++)
    {
      if (secuencia[j] < secuencia[min])
      {
        min = j;
      }
    }
    if (min != i)
    {
      std::swap(secuencia[i], secuencia[min]);
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
            << "2. números, letras y otros caracteres\n"
            << "3. letras, otros caracteres y números\n"
            << "4. números, otros caracteres y letras\n"
            << "5. otros caracteres, letras y números\n"
            << "6. otros caracteres, números y letras\n";
  char opcion;
  std::cin >> opcion;

  std::cout << "Metodo de ordenación:\n"
            << "1. Burbuja\n"
            << "2. Sacudida\n"
            << "Introduzca el número de la opción: ";
  char metodo;
  std::cin >> metodo;
  auto start = std::chrono::high_resolution_clock::now();
  Selection(secuencia, n);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = end - start;

  std::cout << "La secuencia ordenada es: ";
  for (int i = 0; i < n; i++)
  {
    std::cout << secuencia[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "La secuencia ordenada según la opción seleccionada es: ";
  switch (opcion)
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
