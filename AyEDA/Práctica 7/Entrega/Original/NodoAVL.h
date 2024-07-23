// clase NodoAVL a partir de la clase NodoB

#ifndef __NODOAVL_H__
#define __NODOAVL_H__

#include "NodoB.h"

template <class Key>
class NodoAVL : public NodoB<Key>
{
public:

  NodoAVL(Key dato, NodoAVL<Key> *iz = nullptr, NodoAVL<Key> *der = nullptr) : NodoB<Key>(dato_, iz, der), dato_(dato), bal_(0), izquierdo(iz), derecho(der) {}
  ~NodoAVL() {}

  NodoAVL<Key> *izquierdo;
  NodoAVL<Key> *derecho;

  int getBal() const { return bal_; }
  void setBal(int bal) { bal_ = bal; }

  Key getData() const { return dato_; }

private:
  Key dato_;
  int bal_;
};

#endif // __NODOAVL_H__