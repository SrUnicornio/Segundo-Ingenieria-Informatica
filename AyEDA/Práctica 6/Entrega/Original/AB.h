#ifndef __AB_H__
#define __AB_H__

#include <iostream>
#include <queue>
#include "NodoB.h"

template <typename Key>
class AB
{
protected:
  NodoB<Key> *root_; // Pointer to the root of the tree

public:
  // Virtual methods
  virtual bool insert(const Key &data) = 0;
  virtual bool search(const Key &data) const = 0;

  // See the tree
  virtual void inOrder(NodoB<Key> *) const = 0;
  virtual void preOrder(NodoB<Key> *) const = 0;
  virtual void postOrder(NodoB<Key> *) const = 0;
  virtual void levels(NodoB<Key> *) const = 0;

  // Getters and setters
  virtual NodoB<Key> *getRoot() const { return root_; }
  virtual void setRoot(NodoB<Key> *root) { root_ = root; }
};

#endif // __AB_H__