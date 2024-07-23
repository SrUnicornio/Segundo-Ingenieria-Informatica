/**
 * @file TimSort.cc
 * 
 * compilar con: g++ -std=c++23 -o TimSort TimSort.cc
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
void Insercion(Key *secuencia, int left, int right)
{
  for (int i = left + 1; i <= right; i++)
  {
    Key temp = secuencia[i];
    int j = i - 1;
    while (j >= left && secuencia[j] > temp)
    {
      secuencia[j + 1] = secuencia[j];
      j--;
    }
    secuencia[j + 1] = temp;
  }
}

template <class Key>
void merge(Key *secuencia, int l, int m, int r)
{
  int len1 = m - l + 1, len2 = r - m;
  Key L[len1], R[len2];
  for (int i = 0; i < len1; i++) { L[i] = secuencia[l + i]; }
  for (int j = 0; j < len2; j++) { R[j] = secuencia[m + 1 + j]; }
  int i = 0, j = 0, k = l;
  while (i < len1 && j < len2)
  {
    if (L[i] <= R[j])
    {
      secuencia[k] = L[i];
      i++;
    }
    else
    {
      secuencia[k] = R[j];
      j++;
    }
    k++;
  }
  while (i < len1)
  {
    secuencia[k] = L[i];
    i++;
    k++;
  }
  while (j < len2)
  {
    secuencia[k] = R[j];
    j++;
    k++;
  }
}

template <class Key>
void TimSort(Key *secuencia, int n)
{
  // Implementación del algoritmo TimSort

  for (int i = 0; i < n; i++)
  {
    Insercion(secuencia, i, i + 1);
  }

  for (int i = 0; i < n; i++)
  {
    for (int j = i + 1; j < n; j++)
    {
      merge(secuencia, i, (i + j) / 2, j);
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
            << "2. letras, otros caracteres y números\n"
            << "3. números, letras y otros caracteres\n"
            << "4. números, otros caracteres y letras\n"
            << "5. otros caracteres, letras y números\n"
            << "6. otros caracteres, números y letras\n"
            << "Introduzca el número de la opción: ";
  char orden;
  std::cin >> orden;

  auto start = std::chrono::high_resolution_clock::now();
  // HeapSort(secuencia, n);
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
