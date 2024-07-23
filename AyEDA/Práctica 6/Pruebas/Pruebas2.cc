/**
 * Creacion de arboles binarios (TDA)
 *
 * Arboles binarios equilibrado (ABE)
 * Arboles binarios de busqueda (ABB)
 */

#include <algorithm>
#include <cmath>
#include <deque>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>

template <class Key>
class NodoB
{
public:
  NodoB(const Key &k, NodoB *izq = nullptr, NodoB *der = nullptr) : data_(k), izq_(nullptr), der_(nullptr){};
  ~NodoB()
  {
    delete izq_;
    delete der_;
  }
  Key getData() const { return data_; }
  NodoB *getIzq() const { return izq_; }
  NodoB *getDer() const { return der_; }
  void setData(const Key &k) { data_ = k; }
  void setIzq(const Key &k) { izq_ = k; }
  void setDer(const Key &k) { der_ = k; }

private:
  Key data_;
  NodoB<Key> *izq_;
  NodoB<Key> *der_;
};

template <class Key>
class AB
{
public:
  virtual bool insert(const Key &k) = 0;
  virtual bool search(const Key &k) const = 0;
  virtual void inOrden(NodoB<Key> *nodo) const = 0;
  virtual NodoB<Key> *getRaiz() const = 0;
};

template <class Key>
class ABE : public AB<Key>
{
public:
  ABE() : raiz_(nullptr){};
  int tamRama(NodoB<Key> *rama)
  {
    if (rama == nullptr)
      return 0;
    return 1 + tamRama(rama->getIzq()) + tamRama(rama->getDer());
  }
  bool insert(const Key &k)
  {
    if (search(k))
      return false;
    if (raiz_ == nullptr)
      raiz_ = new NodoB<Key>(k);
    else
      insertNode(k, raiz_);
    return true;
  }
  void insertNode(const Key &k, NodoB<Key> *rama)
  {
    if (tamRama(rama->getIzq()) <= tamRama(rama->getDer()))
    {
      if (rama->getIzq() != nullptr)
        insertNode(k, rama->getIzq());
      else
        rama->setIzq(new NodoB<Key>(k));
    }
    else
    {
      if (rama->getDer() != nullptr)
        insertNode(k, rama->getDer());
      else
        rama->setDer(new NodoB<Key>(k));
    }
  }
  bool search(const Key &k)
  {
    if (raiz_ == nullptr)
      return false;
    return searchNode(k, raiz_);
  }
  bool searchNode(const Key &k, NodoB<Key> *rama)
  {
    if (rama == nullptr)
      return false;
    if (k == rama->getData())
      return true;
    return searchNode(k, rama->getIzq()) || searchNode(k, rama->getDer());
  }
  void inOrden(NodoB<Key> *nodo)
  {
    if (nodo == nullptr)
      return;
    inOrden(nodo->getIzq());
    std::cout << nodo->getData() << " ";
    inOrden(nodo->getDer());
  }
  NodoB<Key> *getRaiz() { return raiz_; };

private:
  NodoB<Key> *raiz_;
};

template <class Key>
class ABB : public AB<Key>
{
public:
  ABB() : raiz_(nullptr){};
  ~ABB() { podar(raiz_); }
  bool insert(const Key &k) { insertNode(k, raiz_); }
  bool search(const Key &k) { return searchNode(k, raiz_); }
  bool remove(const Key &k) { removeNode(k, raiz_); }
  void inOrden(NodoB<Key> *nodo) const
  {
    if (nodo == nullptr)
      return;
    inOrden(nodo->getIzq());
    std::cout << nodo->getData() << " ";
    inOrden(nodo->getDer());
  }
  NodoB<Key> *getRaiz() { return raiz_; };

private:
  NodoB<Key> *raiz_;

  void podar(NodoB<Key> *nodo)
  {
    if (nodo == nullptr)
      return;
    podar(nodo->getIzq());
    podar(nodo->getDer());
    delete nodo;
    nodo = nullptr;
  }
  int size() { return size_rama(raiz_); }
  int size_rama(NodoB<Key> *nodo)
  {
    if (nodo == nullptr)
      return 0;
    return 1 + size_rama(nodo->getIzq()) + size_rama(nodo->getDer());
  }
  int altura() { return altura_nodo(raiz_); }
  int altura_nodo(NodoB<Key> *nodo)
  {
    if (nodo == nullptr)
      return 0;
    return 1 + std::max(altura_nodo(nodo->getIzq()), altura_nodo(nodo->getDer()));
  }
  void insertNode(const Key &k, NodoB<Key> *nodo)
  {
    if (nodo == nullptr)
    {
      nodo = new NodoB<Key>(k);
      if (raiz_ == nullptr)
        raiz_ = nodo;
    }
    else if (k < nodo->getData())
    {
      if (nodo->getIzq() != nullptr)
        insertNode(k, nodo->getIzq());
      else
        nodo->setIzq(new NodoB<Key>(k));
    }
    else if (k > nodo->getData())
    {
      if (nodo->getDer() != nullptr)
        insertNode(k, nodo->getDer());
      else
        nodo->setDer(new NodoB<Key>(k));
    }
  }
  bool searchNode(const Key &k, NodoB<Key> *nodo)
  {
    if (nodo == nullptr)
      return false;
    if (k == nodo->getData())
      return true;
    if (k < nodo->getData())
      return searchNode(k, nodo->getIzq());
    return searchNode(k, nodo->getDer());
  }
  void removeNode(const Key &k, NodoB<Key> *nodo)
  {
    if (nodo == nullptr)
      return;
    if (k < nodo->getData())
      removeNode(k, nodo->getIzq());
    else if (k > nodo->getData())
      removeNode(k, nodo->getDer());
    else
      sustituir(nodo, nodo->getIzq());
  }
  void sustituir(NodoB<Key> *eliminado, NodoB<Key> *sustituto)
  {
    if (sustituto == nullptr)
    {
      NodoB<Key> *aux = eliminado;
      eliminado = eliminado->getDer();
      delete aux;
    }
    else if (sustituto->getDer() != nullptr)
      sustituir(eliminado, sustituto->getDer());
    else
    {
      eliminado->setData(sustituto->getData());
      eliminado = sustituto;
      sustituto = sustituto->getIzq();
    }
  }
};

int main()
{

  return 0;
}