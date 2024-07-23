/**
 * @file Ordenacion_MergeSort.cc
 * 
 * compilar con: g++ -std=c++23 -o Ordenacion_MergeSort Ordenacion_MergeSort.cc
*/

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

template <typename Key>
void toLower(Key& value) {
    // No hacemos nada por defecto, ya que no todos los tipos de datos son convertibles a minúsculas
}

template <>
void toLower(std::string& value) {
    for (char& c : value) {
        if (std::isalpha(c)) { // Solo convertimos caracteres alfabéticos
            c = std::tolower(c);
        }
    }
}

template <class Key>
void merge(Key* secuencia, int inicio, int medio, int fin) 
{
  int i = inicio;
  int j = medio + 1;
  int k = inicio;
  Key aux[fin + 1];
  while (i <= medio && j <= fin) {
    if (secuencia[i] < secuencia[j]) {
      aux[k] = secuencia[i];
      i++;
    } else {
      aux[k] = secuencia[j];
      j++;
    }
    k++;
  }
  if (i > medio) {
    while (j <= fin) {
      aux[k] = secuencia[j];
      j++;
      k++;
    }
  } else {
    while (i <= medio) {
      aux[k] = secuencia[i];
      i++;
      k++;
    }
  }
  for (int l = inicio; l <= fin; l++) {
    secuencia[l] = aux[l];
  }
}

template <class Key>
void mergeSort(Key* secuencia, int inicio, int fin) {
  if (inicio < fin) {
    int medio = (fin + inicio) / 2;
    mergeSort(secuencia, inicio, medio);
    mergeSort(secuencia, medio + 1, fin);
    merge(secuencia, inicio, medio, fin);
  }
}

template <class Key, class Func>
void imprimirSecuencia(const Key* secuencia, int n, Func condicion) {
    for (int i = 0; i < n; i++) {
        if (condicion(secuencia[i][0])) {
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
            << "2. letras, otros caracteres y números\n"
            << "3. números, letras y otros caracteres\n"
            << "4. números, otros caracteres y letras\n"
            << "5. otros caracteres, letras y números\n"
            << "6. otros caracteres, números y letras\n"
            << "Introduzca el número de la opción: ";
  char orden;
  std::cin >> orden;

  auto start = std::chrono::high_resolution_clock::now();
  mergeSort(secuencia, 0, n - 1);
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
