#ifndef __ABE_H__
#define __ABE_H__

#include <iostream>
#include <queue>
#include "AB.h"

template <typename Key>
class ABE : public AB<Key>
{
public:
  // Default constructor
  ABE() : AB<Key>() {}

  // Destructor
  ~ABE() {}

  int tamRama(NodoB<Key> *rama)
  {
    if (rama == nullptr)
      return 0;
    return 1 + tamRama(rama->getleft()) + tamRama(rama->getright());
  }

  bool insert(const Key &data)
  {
    if (search(data))
      return false;
    if (this->getRoot() == nullptr)
    {
      this->setRoot(new NodoB<Key>(data));
      return true;
    }
    else
    {
      insertNode(data, this->getRoot());
      return true;
    }
  }

  void insertNode(const Key &data, NodoB<Key> *rama)
  {
    if (tamRama(rama->getleft()) <= tamRama(rama->getright()))
    {
      if (rama->getleft() != nullptr)
        insertNode(data, rama->getleft());
      else
        rama->setleft(new NodoB<Key>(data));
    }
    else
    {
      if (rama->getright() != nullptr)
        insertNode(data, rama->getright());
      else
        rama->setright(new NodoB<Key>(data));
    }
  }

  // Search a node in the balanced binary tree
  bool search(const Key &data) const
  {
    if (this->getRoot() == nullptr)
      return false;
    return searchNode(data, this->getRoot());
  }

  bool searchNode(const Key &data, NodoB<Key> *rama) const
  {
    if (rama == nullptr)
      return false;
    if (data == rama->getData())
      return true;
    return searchNode(data, rama->getleft()) || searchNode(data, rama->getright());
  }

  // In-order traversal of the balanced binary tree
  void inOrder(NodoB<Key> *node) const
  {
    if (node != nullptr)
    {
      inOrder(node->getleft());
      std::cout << node->getData() << " ";
      inOrder(node->getright());
    }
  }

  bool inOrderSearch(NodoB<Key> *node, const Key &data) const
  {
    if (node != nullptr)
    {
      inOrderSearch(node->getleft(), data);
      if (node->getData() == data)
        return true;
      inOrderSearch(node->getright(), data);
    }
    return false;
  }

  // Pre-order traversal of the balanced binary tree
  void preOrder(NodoB<Key> *node) const
  {
    if (node != nullptr)
    {
      std::cout << node->getData() << " ";
      preOrder(node->getleft());
      preOrder(node->getright());
    }
  }

  // Post-order traversal of the balanced binary tree
  void postOrder(NodoB<Key> *node) const
  {
    if (node != nullptr)
    {
      postOrder(node->getleft());
      postOrder(node->getright());
      std::cout << node->getData() << " ";
    }
  }

  // Level traversal of the balanced binary tree
  void levels(NodoB<Key> *node) const
  {
    NodoB<Key> *raiz = this->getRoot();
    std::deque<std::pair<NodoB<Key> *, int>> Q;
    NodoB<Key> *aux;
    int nivel = 0;
    int nivel_actual = 0;
    Q.push_back(std::pair<NodoB<Key> *, int>(raiz, 0));
    std::cout << "\n Nivel 0:";
    while (!Q.empty())
    {
      aux = Q.front().first;
      nivel = Q.front().second;
      Q.pop_front();
      if (nivel > nivel_actual)
      {
        nivel_actual = nivel;
        std::cout << "\n Nivel " << nivel_actual << ":";
      }
      if (aux != nullptr)
      {
        std::cout << "[" << aux->getData() << "] ";
        Q.push_back(std::pair<NodoB<Key> *, int>(aux->getleft(), nivel + 1));
        Q.push_back(std::pair<NodoB<Key> *, int>(aux->getright(), nivel + 1));
      }
      else
      {
        std::cout << "[.] ";
      }
    }
  }

  bool levelsSearch(NodoB<Key> *node, const Key &data) const
  {
    NodoB<Key> *raiz = this->getRoot();
    std::deque<std::pair<NodoB<Key> *, int>> Q;
    NodoB<Key> *aux;
    int nivel = 0;
    int nivel_actual = 0;
    Q.push_back(std::pair<NodoB<Key> *, int>(raiz, 0));
    while (!Q.empty())
    {
      aux = Q.front().first;
      nivel = Q.front().second;
      Q.pop_front();
      if (nivel > nivel_actual)
      {
        nivel_actual = nivel;
      }
      if (aux != nullptr)
      {
        if (aux->getData() == data)
          return true;
        Q.push_back(std::pair<NodoB<Key> *, int>(aux->getleft(), nivel + 1));
        Q.push_back(std::pair<NodoB<Key> *, int>(aux->getright(), nivel + 1));
      }
    }
    return false;
  }
};

#endif // __ABE_H__