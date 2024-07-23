#include <cstring> // Para strlen
#include <iostream>

const int SIZE = 16;

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
  int count[256] = {0}; // 256 para soportar caracteres ASCII extendidos

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
void RadixSortAlfanumerico(Key secuencia[], int n)
{
  // Pasar strings a char arrays
  char secuenciaChar[n][SIZE];
  for (int i = 0; i < n; i++)
  {
    strcpy(secuenciaChar[i], secuencia[i].c_str());
  }
  int maxLength = getMaxLength(secuenciaChar, n);

  // Comenzamos desde el último caracter y vamos hacia atrás
  for (int exp = maxLength - 1; exp >= 0; exp--)
  {
    CountSortChar(secuenciaChar, n, exp);
  }

  // Pasar char arrays a strings
  for (int i = 0; i < n; i++)
  {
    secuencia[i] = secuenciaChar[i];
  }
}

// Función para imprimir la secuencia alfanumérica ordenada
template <class Key>
void imprimirSecuenciaAlfanumerica(Key secuencia[], int n)
{
  for (int i = 0; i < n; i++)
  {
    std::cout << secuencia[i] << " ";
  }
  std::cout << std::endl;
}

int main()
{
  std::string secuencia[] = {"banana", "apple", "orange", "grape", "pineapple", "kiwi", "pear", "234", "123", "#", "|", "qwertyuiopnlkjh"};
  int n = sizeof(secuencia) / sizeof(secuencia[0]);
  RadixSortAlfanumerico(secuencia, n);
  imprimirSecuenciaAlfanumerica(secuencia, n);
  return 0;
}