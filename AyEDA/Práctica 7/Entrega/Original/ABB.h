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

  // Insert a new node in the binary search tree
  void insertNode(const Key &data, NodoB<Key> *node)
  {
    if (data < node->getData())
    {
      if (node->getleft() != nullptr)
        insertNode(data, node->getleft());
      else
        node->setleft(new NodoB<Key>(data));
    }
    else
    {
      if (node->getright() != nullptr)
        insertNode(data, node->getright());
      else
        node->setright(new NodoB<Key>(data));
    }
  }
  // Search a node in the binary search tree
  bool search(const Key &data) const
  {
    if (this->getRoot() == nullptr)
      return false;
    return searchNode(data, this->getRoot());
  }

  // Search a node in the binary search tree
  bool searchNode(const Key &data, NodoB<Key> *node) const
  {
    if (node == nullptr)
      return false;
    if (data == node->getData())
      return true;
    else if (data < node->getData())
      return searchNode(data, node->getleft());
    else
      return searchNode(data, node->getright());
  }

  // In-order traversal of the binary search tree
  void inOrder() const override
  {
    inOrder(this->getRoot());
  }
  void inOrder(NodoB<Key> *node) const
  {
    if (node != nullptr)
    {
      inOrder(node->getleft());
      std::cout << node->getData() << " ";
      inOrder(node->getright());
    }
  }

  // Pre-order traversal of the binary search tree
  void preOrder() const override
  {
    preOrder(this->getRoot());
  }
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
  void postOrder() const override
  {
    postOrder(this->getRoot());
  }
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
  void levels() const override
  {
    levels(this->getRoot());
  }
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

  std::queue<Key> inOrderQueue() const override
  {
    std::queue<Key> q;
    inOrderQueue(this->getRoot(), q);
    return q;
  }

  void inOrderQueue(NodoB<Key> *node, std::queue<Key> &q) const
  {
    if (node != nullptr)
    {
      inOrderQueue(node->getleft(), q);
      q.push(node->getData());
      inOrderQueue(node->getright(), q);
    }
  }
};

#endif // __ABB_H__