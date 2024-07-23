/**
 * @file RadixSort.cc
 * 
 * Compilar con: g++ -std=c++23 -o RadixSort RadixSort.cc
*/

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <algorithm> // Para transform
#include <cstring> // Para strlen


const int SIZE = 256;

// Función para encontrar la longitud máxima de una cadena en la secuencia
int getMaxLength(const char secuencia[][SIZE], int n)
{
  int maxLength = strlen(secuencia[0]); // uso de strlen para obtener la longitud de la cadena
  for (int i = 1; i < n; i++)
  {
    int length = strlen(secuencia[i]);
    if (length > maxLength)
    {
      maxLength = length;
    }
  }
  return maxLength;
}

// Función de CountSort para ordenar la secuencia basada en el caracter en la posición específica
void CountSortChar(char secuencia[][SIZE], int n, int exp)
{
  char output[n][SIZE];
  int count[SIZE] = {0}; // 256 para soportar caracteres ASCII extendidos

  for (int i = 0; i < n; i++)
  {
    count[(unsigned char)secuencia[i][exp]]++;
  }

  for (int i = 1; i < 256; i++)
  {
    count[i] += count[i - 1];
  }

  for (int i = n - 1; i >= 0; i--)
  {
    strcpy(output[count[(unsigned char)secuencia[i][exp]] - 1], secuencia[i]); // uso de strcpy para copiar la cadena
    count[(unsigned char)secuencia[i][exp]]--;
  }

  for (int i = 0; i < n; i++)
  {
    strcpy(secuencia[i], output[i]);
  }
}

// Función principal de RadixSort para ordenar la secuencia alfanumérica
template <class Key>
void RadixSort(Key secuencia[], int n)
{
  // Pasar strings a char arrays
  char sequenceChar[n][SIZE];
  for (int i = 0; i < n; i++)
  {
    strcpy(sequenceChar[i], secuencia[i].c_str());
  }
  int maxLength = getMaxLength(sequenceChar, n);

  for (int exp = maxLength - 1; exp >= 0; exp--)
  {
    CountSortChar(sequenceChar, n, exp);
  }

  for (int i = 0; i < n; i++)
  {
    secuencia[i] = sequenceChar[i];
  }
}

// template <class Key>
void toLower(int &value)
{
  // No hacemos nada por defecto, ya que no todos los tipos de datos son convertibles a minúsculas
}

// template <>
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
void imprimirSecuencia(const Key secuencia[], int n, Func condicion)
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
  int secuenciaN[] = { 345, 721, 425, 572, 836, 467, 672, 194, 365, 236, 891, 746, 431, 834, 247, 529, 216, 389 };
  std::string secuencia[18] = {"·#", "%", "@", "hola"};
  for (int i = 0; i < 14; i++)
  {
    secuencia[i] = std::to_string(secuenciaN[i]);
  }

  int n = sizeof(secuencia) / sizeof(secuencia[0]);

  //convertimos la secuencia a minúsculas si el usuario lo desea
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

  for (int i = 0; i < n; i++)
  {
    std::cout << "Char: " << secuencia[i] << " int: " << secuencia[i] << " \n";
  }

  auto start = std::chrono::high_resolution_clock::now();
  RadixSort(secuencia, n);
  auto end = std::chrono::high_resolution_clock::now();

  std::cout << "Es letra: ";
  for (int i = 0; i < n; i++)
  {
    if (std::isalpha(secuencia[i][0]))
    {
      std::cout << secuencia[i] << " ";
    }
  }
  std::cout << std::endl;

  std::cout << "Es número: ";
  for (int i = 0; i < n; i++)
  {
    if (std::isdigit(secuencia[i][0]))
    {
      std::cout << secuencia[i] << " ";
    }
  }
  std::cout << std::endl;

  std::cout << "Es otro caracter: ";
  for (int i = 0; i < n; i++)
  {
    if (!std::isalnum(secuencia[i][0]))
    {
      std::cout << secuencia[i] << " ";
    }
  }
  std::cout << std::endl;

  std::cout << "secuencia ordenada: ";

  // Mostramos la secuencia ordenada como quiere el usuario
  switch (orden)
  {
    case '1':
      imprimirSecuencia(secuencia, n, [](char c)
                        { return std::isalpha(c); });
      imprimirSecuencia(secuencia, n, [](char c)
                        { return std::isdigit(c); });
      imprimirSecuencia(secuencia, n, [](char c)
                        { return !std::isalnum(c); });
      break;
    case '2':
      imprimirSecuencia(secuencia, n, [](char c)
                        { return std::isalpha(c); });
      imprimirSecuencia(secuencia, n, [](char c)
                        { return !std::isalnum(c); });
      imprimirSecuencia(secuencia, n, [](char c)
                        { return std::isdigit(c); });
      break;
    case '3':
      imprimirSecuencia(secuencia, n, [](char c)
                        { return std::isdigit(c); });
      imprimirSecuencia(secuencia, n, [](char c)
                        { return std::isalpha(c); });
      imprimirSecuencia(secuencia, n, [](char c)
                        { return !std::isalnum(c); });
      break;
    case '4':
      imprimirSecuencia(secuencia, n, [](char c)
                        { return std::isdigit(c); });
      imprimirSecuencia(secuencia, n, [](char c)
                        { return !std::isalnum(c); });
      imprimirSecuencia(secuencia, n, [](char c)
                        { return std::isalpha(c); });
      break;
    case '5':
      imprimirSecuencia(secuencia, n, [](char c)
                        { return !std::isalnum(c); });
      imprimirSecuencia(secuencia, n, [](char c)
                        { return std::isalpha(c); });
      imprimirSecuencia(secuencia, n, [](char c)
                        { return std::isdigit(c); });
      break;
    case '6':
      imprimirSecuencia(secuencia, n, [](char c)
                        { return !std::isalnum(c); });
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
