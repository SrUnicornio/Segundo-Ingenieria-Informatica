#include <iostream>
// cola
#include <queue>
// Random
#include <cstdlib>
// Time
#include <ctime>

class nodoB
{
public:
  // Constructor:
  nodoB(const int dat, nodoB *izq = NULL, nodoB *der = NULL) : dato(dat), izdo(izq), dcho(der) {}

  // Miembros:
  int dato;    // dato: Cualquier tipo de
  nodoB *izdo; //       valor a almacenar
  nodoB *dcho;
};

class ArbolB
{
private:
  nodoB *raiz;

public:
  ArbolB() : raiz(NULL) {}
  ~ArbolB() { Podar(raiz); }
  // getters
  nodoB *Raiz() { return raiz; }
  void Podar(nodoB *&nodo)
  {
    if (nodo == NULL)
      return;
    Podar(nodo->izdo); // Podar subarbol izquierdo
    Podar(nodo->dcho); // Podar subarbol derecho
    delete nodo;       // Eliminar nodo
    nodo = NULL;
  }
  bool EsVacio(nodoB *nodo)
  {
    return nodo == NULL;
  }
  bool EsHoja(nodoB *nodo)
  {
    return !nodo->dcho && !nodo->izdo;
  }
  const int Tam() { return TamRama(raiz); }
  const int TamRama(nodoB *nodo)
  {
    if (nodo == NULL)
      return 0;
    return (1 + TamRama(nodo->izdo) + TamRama(nodo->dcho));
  }
  const int Alt() { return AltN(raiz); }
  const int AltN(nodoB *nodo)
  {
    if (nodo == NULL)
      return 0;
    int alt_i = AltN(nodo->izdo);
    int alt_d = AltN(nodo->dcho);
    if (alt_d > alt_i)
      return ++alt_d;
    else
      return ++alt_i;
  }
  const bool Equilibrado()
  {
    return EquilibrioRama(raiz);
  }

  const bool EquilibrioRama(nodoB *nodo)
  {
    if (nodo == NULL)
      return true;
    int eq = TamRama(nodo->izdo) - TamRama(nodo->dcho);
    switch (eq)
    {
    case -1:
    case 0:
    case 1:
      return EquilibrioRama(nodo->izdo) &&
             EquilibrioRama(nodo->dcho);
    default:
      return false;
    }
  }
  void InsertaEquil(const int dato)
  {
    if (raiz == NULL)
      raiz = new nodoB(dato, NULL, NULL);
    else
      InsertaEquilRama(dato, raiz);
  }
  void InsertaEquilRama(const int dato, nodoB *nodo)
  {
    if (TamRama(nodo->izdo) <= TamRama(nodo->dcho))
    {
      if (nodo->izdo != NULL)
        InsertaEquilRama(dato, nodo->izdo);
      else
        nodo->izdo = new nodoB(dato, NULL, NULL);
    }
    else
    {
      if (nodo->dcho != NULL)
        InsertaEquilRama(dato, nodo->dcho);
      else
        nodo->dcho = new nodoB(dato, NULL, NULL);
    }
  }
};

void MostrarArbol(nodoB *nodo, int nivel)
{
  if (nodo == NULL)
    return;
  std::queue<nodoB *> cola;
  cola.push(nodo);
  int nivel_actual = 0;
  while (!cola.empty())
  {
    std::cout << "Nivel " << nivel_actual << ": ";
    int n = cola.size();
    for (int i = 0; i < n; i++)
    {
      nodoB *nodo = cola.front();
      cola.pop();
      if (nodo != nullptr)
      {
        std::cout << "[" << nodo->dato << "] ";
        cola.push(nodo->izdo);
        cola.push(nodo->dcho);
      }
      else
      {
        std::cout << "[.] ";
      }
    }
    std::cout << std::endl;
    nivel_actual++;
  }
  std::cout << std::endl;
}

int main()
{
  ArbolB abe;
  // Seed
  // srand(time(NULL));
  // int n;
  // for (int i = 0; i < 20; i++)
  // {
  //   n = rand() % 100;
  //   abe.InsertaEquil(n);
  // }
  abe.InsertaEquil(18);
  abe.InsertaEquil(45);
  abe.InsertaEquil(28);
  abe.InsertaEquil(13);
  abe.InsertaEquil(14);
  abe.InsertaEquil(17);
  abe.InsertaEquil(34);
  abe.InsertaEquil(21);
  abe.InsertaEquil(33);
  abe.InsertaEquil(42);
  abe.InsertaEquil(43);
  abe.InsertaEquil(26);
  abe.InsertaEquil(12);
  abe.InsertaEquil(21);
  abe.InsertaEquil(23);
  std::cout << "Altura del árbol: " << abe.Alt() << std::endl;
  std::cout << "Número de nodos del árbol: " << abe.Tam() << std::endl;
  if (abe.Equilibrado())
    std::cout << "El árbol está equilibrado." << std::endl;
  else
    std::cout << "El árbol no está equilibrado." << std::endl;

  std::cout << "Mostrando árbol:" << std::endl;
  MostrarArbol(abe.Raiz(), 0);
  return 0;
}