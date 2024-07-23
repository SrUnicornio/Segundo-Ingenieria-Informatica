#ifndef __ABB_H__
#define __ABB_H__

#include <iostream>
#include <queue>
#include "AB.h"

// Class for the binary search tree
template <typename Key>
class ABB : public AB<Key>
{
public:
  // Default constructor
  ABB() : AB<Key>() {}

  // Destructor
  ~ABB() {}

  // Insert a new node in the binary search tree
  bool insert(const Key &data)
  {
    NodoB<Key> *newNode = new NodoB<Key>(data);
    if (this->getRoot() == nullptr)
    {
      this->setRoot(newNode);
      return true;
    }

    NodoB<Key> *current = this->getRoot();
    NodoB<Key> *parent = nullptr;
    while (current != nullptr)
    {
      parent = current;
      if (data == current->getData())
      {
        delete newNode;
        return false;
      }
      else if (data < current->getData())
        current = current->getleft();
      else
        current = current->getright();
    }

    if (data < parent->getData())
      parent->setleft(newNode);
    else
      parent->setright(newNode);
    return true;
  }

  // Search a node in the binary search tree
  bool search(const Key &data) const
  {
    NodoB<Key> *current = this->getRoot();
    while (current != nullptr)
    {
      if (current->getData() == data)
        return true;
      else if (data < current->getData())
        current = current->getleft();
      else
        current = current->getright();
    }
    return false;
  }

  // In-order traversal of the binary search tree
  void inOrder(NodoB<Key> *node) const
  {
    if (node != nullptr)
    {
      inOrder(node->getleft());
      std::cout << node->getData() << " ";
      inOrder(node->getright());
    }
  }

  // In-order search traversal of the binary search tree
  bool inOrderSearch(NodoB<Key> *node, const Key &data) const
  {
    if (node != nullptr)
    {
      if (inOrderSearch(node->getleft(), data))
        return true;
      if (node->getData() == data)
        return true;
      if (inOrderSearch(node->getright(), data))
        return true;
    }
    return false;
  }

  // Pre-order traversal of the binary search tree
  void preOrder(NodoB<Key> *node) const
  {
    if (node != nullptr)
    {
      std::cout << node->getData() << " ";
      preOrder(node->getleft());
      preOrder(node->getright());
    }
  }

  // Post-order traversal of the binary search tree
  void postOrder(NodoB<Key> *node) const
  {
    if (node != nullptr)
    {
      postOrder(node->getleft());
      postOrder(node->getright());
      std::cout << node->getData() << " ";
    }
  }

  // Level traversal of the binary search tree
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

#endif // __ABB_H__