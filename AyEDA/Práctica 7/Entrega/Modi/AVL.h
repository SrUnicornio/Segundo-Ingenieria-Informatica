#ifndef __AVL_H__
#define __AVL_H__

#include "ABB.h"
#include "NodoAVL.h"
#include <deque>
#include <iostream>

template <class Key>
class AVL : public ABB<Key>
{
private:
  NodoAVL<Key> *raiz;

  // Busqueda
  bool searchNode(NodoAVL<Key> *node, Key data) const
  {
    if (node == nullptr)
      return false;

    if (data == node->getData())
      return true;

    if (data < node->getData())
      return searchNode(node->izquierdo, data);

    return searchNode(node->derecho, data);
  }

  // insert
  void insertBal(NodoAVL<Key> *&node, NodoAVL<Key> *nuevo, bool &crece)
  {
    if (node == nullptr)
    {
      node = nuevo;
      crece = true;
    }
    else if (nuevo->getData() < node->getData())
    {
      insertBal(node->izquierdo, nuevo, crece);
      if (crece)
        insert_reBalLeft(node, crece);
    }
    else
    {
      insertBal(node->derecho, nuevo, crece);
      if (crece)
        insert_reBalRight(node, crece);
    }
  }
  void insert_reBalLeft(NodoAVL<Key> *&node, bool &crece)
  {
    NodoAVL<Key> *node1;
    switch (node->getBal())
    {
    case -1:
      node->setBal(0);
      crece = false;
      break;

    case 0:
      node->setBal(1);
      break;
    case 1:
      node1 = node->izquierdo;
      if (node1->getBal() == 1)
        rotationLL(node);
      else
        rotationLR(node);
      crece = false;
      break;
    }
  }
  void insert_reBalRight(NodoAVL<Key> *&node, bool &crece)
  {
    NodoAVL<Key> *node1;
    switch (node->getBal())
    {
    case 1:
      node->setBal(0);
      crece = false;
      break;
    case 0:
      node->setBal(-1);
      break;
    case -1:
      node1 = node->derecho;
      if (node1->getBal() == -1)
        rotationRR(node);
      else
        rotationRL(node);
      crece = false;
      break;
    }
  }

  // rotationes
  void rotationLL(NodoAVL<Key> *&node)
  {
    if (showTrace)
    {
      levels(raiz);
      std::cout << "\nRotation LL in [" << node->getData().getNombre() << " " << node->getData().calcularMediaStas() << "(" << node->getBal() << ")]\n";
    }
    NodoAVL<Key> *node1 = node->izquierdo;
    node->izquierdo = node1->derecho;
    node1->derecho = node;

    if (node1->getBal() == 1)
    {
      node->setBal(0);
      node1->setBal(0);
    }
    else
    {
      node->setBal(1);
      node1->setBal(-1);
    }

    node = node1;
  }
  void rotationRR(NodoAVL<Key> *&node)
  {
    if (showTrace)
    {
      levels(raiz);
      std::cout << "\nRotation RR in [" << node->getData().getNombre() << " " << node->getData().calcularMediaStas() << "(" << node->getBal() << ")]\n";
    }
    NodoAVL<Key> *node1 = node->derecho;
    node->derecho = node1->izquierdo;
    node1->izquierdo = node;

    if (node1->getBal() == -1)
    {
      node->setBal(0);
      node1->setBal(0);
    }
    else
    {
      node->setBal(-1);
      node1->setBal(1);
    }

    node = node1;
  }
  void rotationLR(NodoAVL<Key> *&node)
  {
    if (showTrace)
    {
      levels(raiz);
      std::cout << "\nRotation LR in [" << node->getData().getNombre() << " " << node->getData().calcularMediaStas() << "(" << node->getBal() << ")]\n";
    }
    NodoAVL<Key> *node1 = node->izquierdo;
    NodoAVL<Key> *node2 = node1->derecho;

    node->izquierdo = node2->derecho;
    node2->derecho = node;
    node1->derecho = node2->izquierdo;
    node2->izquierdo = node1;

    if (node2->getBal() == -1)
      node1->setBal(1);
    else
      node1->setBal(0);

    if (node2->getBal() == 1)
      node->setBal(-1);
    else
      node->setBal(0);

    node2->setBal(0);
    node = node2;
  }
  void rotationRL(NodoAVL<Key> *&node)
  {
    if (showTrace)
    {
      levels(raiz);
      std::cout << "\nRotation RL in [" << node->getData().getNombre() << " " << node->getData().calcularMediaStas() << "(" << node->getBal() << ")]\n";
    }
    NodoAVL<Key> *node1 = node->derecho;
    NodoAVL<Key> *node2 = node1->izquierdo;

    node->derecho = node2->izquierdo;
    node2->izquierdo = node;
    node1->izquierdo = node2->derecho;
    node2->derecho = node1;

    if (node2->getBal() == 1)
      node1->setBal(-1);
    else
      node1->setBal(0);

    if (node2->getBal() == -1)
      node->setBal(1);
    else
      node->setBal(0);

    node2->setBal(0);
    node = node2;
  }

  bool showTrace;

public:
  AVL(const bool &trace = false) : ABB<Key>(), raiz(nullptr), showTrace(trace) {}
  ~AVL();

  // insert
  bool insert(const Key &data)
  {
    std::cout << "\nInserting: " << data << std::endl;
    if (search(data))
      return false;
    if (raiz == nullptr)
    {
      raiz = new NodoAVL<Key>(data);
      return true;
    }
    NodoAVL<Key> *nuevo = new NodoAVL<Key>(data);
    bool crece = false;
    insertBal(raiz, nuevo, crece);
    return true;
  }

  bool search(const Key &data) const
  {
    levels(raiz);
    if (raiz == nullptr)
      return false;
    return searchNode(raiz, data);
  }

  // Recorridos
  void preOrder() const override
  {
    preOrder(raiz);
    std::cout << std::endl;
  }
  void preOrder(NodoAVL<Key> *node) const
  {
    if (node != nullptr)
    {
      std::cout << node->getData() << " ";
      preOrder(node->izquierdo);
      preOrder(node->derecho);
    }
  }
  void inOrder() const override
  {
    inOrder(raiz);
    std::cout << std::endl;
  }
  void inOrder(NodoAVL<Key> *node) const
  {
    if (node != nullptr)
    {
      inOrder(node->izquierdo);
      std::cout << node->getData() << " ";
      inOrder(node->derecho);
    }
  }
  void postOrder() const override
  {
    postOrder(raiz);
    std::cout << std::endl;
  }
  void postOrder(NodoAVL<Key> *node) const
  {
    if (node != nullptr)
    {
      postOrder(node->izquierdo);
      postOrder(node->derecho);
      std::cout << node->getData() << " ";
    }
  }
  void levels() const override
  {
    levels(raiz);
    std::cout << std::endl;
  }
  void levels(const NodoAVL<Key> *node) const
  {
    NodoAVL<Key> *raiz = const_cast<NodoAVL<Key> *>(node);
    std::deque<std::pair<NodoAVL<Key> *, int>> Q;
    NodoAVL<Key> *aux;
    int nivel = 0;
    int nivel_actual = 0;
    Q.push_back(std::pair<NodoAVL<Key> *, int>(raiz, 0));
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
        if (showTrace == true)
        {
          std::cout << "[" << aux->getData().getNombre() << " " << aux->getData().calcularMediaStas() << "(" << aux->getBal() << ")] ";
        }
        else
          std::cout << "[" << aux->getData().getNombre() << " " << aux->getData().calcularMediaStas() << "] ";
        Q.push_back(std::pair<NodoAVL<Key> *, int>(aux->izquierdo, nivel + 1));
        Q.push_back(std::pair<NodoAVL<Key> *, int>(aux->derecho, nivel + 1));
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
    inOrderQueue(raiz, q);
    return q;
  }

  void inOrderQueue(NodoAVL<Key> *node, std::queue<Key> &q) const
  {
    if (node != nullptr)
    {
      inOrderQueue(node->izquierdo, q);
      q.push(node->getData());
      inOrderQueue(node->derecho, q);
    }
  }
};

#endif // __AVL_H__