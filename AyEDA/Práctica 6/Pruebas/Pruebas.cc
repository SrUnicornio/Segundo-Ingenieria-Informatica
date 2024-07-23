/**
 * @file Pruebas.cc
 *
 * @brief Implementación de del TDA Árbol Binario
 *        ABE (Árbol Binario Equilibrado) // Falla en la implementación insert
 *        ABB (Árbol Binario de Búsqueda)
 *
 * Uso de polimorfismo dinámico y sobrecarga de operadores
 *
 * Compilar con g++ -std=c++23 -o Pruebas Pruebas.cc
 */

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <queue>

// Clase NodoB (Nodo Binario)
template <typename Key>
class NodoB
{
private:
  Key data_;     // Contain the data of the node
  NodoB *left_;  // Pointer to the left child
  NodoB *right_; // Pointer to the right child

public:
  // Default constructor
  NodoB() : left_(nullptr), right_(nullptr) {}
  // Constructor
  NodoB(const Key &data) : data_(data), left_(nullptr), right_(nullptr) {}

  // Destructor
  ~NodoB()
  {
    delete left_;
    delete right_;
  }

  // Getters
  const Key &getDato() const { return data_; }
  NodoB *getIzq() const { return left_; }
  NodoB *getDer() const { return right_; }

  // Setters
  void setDato(Key data) { data_ = data; }
  void setIzq(NodoB *left) { left_ = left; }
  void setDer(NodoB *right) { right_ = right; }
};

// Abstract class for the binary tree
template <typename Key>
class AB
{
protected:
    NodoB<Key> *root_; // Pointer to the root of the tree

public:
  // Virtual methods
  virtual bool insert(const Key &data) = 0;
  virtual bool search(const Key &data) const = 0;
  // virtual void remove(const Key &data) = 0;
  // virtual void sustitution(NodoB<Key>*, NodoB<Key>*) = 0;

  // See the tree
  virtual void inOrder(NodoB<Key>*) const = 0;
  virtual void preOrder(NodoB<Key>*) const = 0;
  virtual void postOrder(NodoB<Key>*) const = 0;

  // Getters and setters
  virtual NodoB<Key> *getRoot() const { return root_; }
  virtual void setRoot(NodoB<Key> *root) { root_ = root; }

  // Overload operator
  friend std::ostream &operator<<(std::ostream &os, const AB<Key> &ab)
  {
    NodoB<Key> *raiz = ab.getRoot();
    std::deque<std::pair<NodoB<Key> *, int>> Q;
    NodoB<Key> *aux;
    int nivel = 0;
    int nivel_actual = 0;
    Q.push_back(std::pair<NodoB<Key> *, int>(raiz, 0));
    os << "\n Nivel 0:";
    while (!Q.empty())
    {
      aux = Q.front().first;
      nivel = Q.front().second;
      Q.pop_front();
      if (nivel > nivel_actual)
      {
        nivel_actual = nivel;
        os << "\n Nivel " << nivel_actual << ":";
      }
      if (aux != nullptr)
      {
        os << "[" << aux->getDato() << "] ";
        Q.push_back(std::pair<NodoB<Key> *, int>(aux->getIzq(), nivel + 1));
        Q.push_back(std::pair<NodoB<Key> *, int>(aux->getDer(), nivel + 1));
      }
      else
      {
        os << "[.] ";
      }
    }
    return os;
  }
};

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
      if (data == current->getDato())
      {
        delete newNode;
        return false;
      }
      else if (data < current->getDato())
        current = current->getIzq();
      else
        current = current->getDer();
    }

    if (data < parent->getDato())
      parent->setIzq(newNode);
    else
      parent->setDer(newNode);
    return true;
  }

  // Search a node in the binary search tree
  bool search(const Key &data) const
  {
    NodoB<Key> *current = this->getRoot();
    while (current != nullptr)
    {
      if (current->getDato() == data)
        return true;
      else if (data < current->getDato())
        current = current->getIzq();
      else
        current = current->getDer();
    }
    return false;
  }

  // In-order traversal of the binary search tree
  void inOrder(NodoB<Key> *node) const
  {
    if (node != nullptr)
    {
      inOrder(node->getIzq());
      std::cout << node->getDato() << " ";
      inOrder(node->getDer());
    }
  }

  // Pre-order traversal of the binary search tree
  void preOrder(NodoB<Key> *node) const
  {
    if (node != nullptr)
    {
      std::cout << node->getDato() << " ";
      preOrder(node->getIzq());
      preOrder(node->getDer());
    }
  }

  // Post-order traversal of the binary search tree
  void postOrder(NodoB<Key> *node) const
  {
    if (node != nullptr)
    {
      postOrder(node->getIzq());
      postOrder(node->getDer());
      std::cout << node->getDato() << " ";
    }
  }
};

// Class for the balanced binary tree
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
    return 1 + tamRama(rama->getIzq()) + tamRama(rama->getDer());
  }

  // Insert a new node in the balanced binary tree
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

  // Insert a new node in the balanced binary tree
  void insertNode(const Key &data, NodoB<Key> *rama)
  {
    if (tamRama(rama->getIzq()) <= tamRama(rama->getDer()))
    {
      if (rama->getIzq() != nullptr)
        insertNode(data, rama->getIzq());
      else
        rama->setIzq(new NodoB<Key>(data));
    }
    else
    {
      if (rama->getDer() != nullptr)
        insertNode(data, rama->getDer());
      else
        rama->setDer(new NodoB<Key>(data));
    }
  }

  // Search a node in the balanced binary tree
  bool search(const Key &data) const
  {
    if ( this->getRoot() == nullptr)
      return false;
    return searchNode(data, this->getRoot());
  }

  // Search a node in the balanced binary tree
  bool searchNode(const Key &data, NodoB<Key> *rama) const
  {
    if (rama == nullptr)
      return false;
    if (data == rama->getDato())
      return true;
    return searchNode(data, rama->getIzq()) || searchNode(data, rama->getDer());
  }

  // In-order traversal of the balanced binary tree
  void inOrder(NodoB<Key> *node) const
  {
    if (node != nullptr)
    {
      inOrder(node->getIzq());
      std::cout << node->getDato() << " ";
      inOrder(node->getDer());
    }
  }

  // Pre-order traversal of the balanced binary tree
  void preOrder(NodoB<Key> *node) const
  {
    if (node != nullptr)
    {
      std::cout << node->getDato() << " ";
      preOrder(node->getIzq());
      preOrder(node->getDer());
    }
  }

  // Post-order traversal of the balanced binary tree
  void postOrder(NodoB<Key> *node) const
  {
    if (node != nullptr)
    {
      postOrder(node->getIzq());
      postOrder(node->getDer());
      std::cout << node->getDato() << " ";
    }
  }
};


void testABE(ABE<int> &abe)
{
  // 18 45 28 13 14 17 34 21 33 42 43 26 12 21 23
  abe.insert(18);
  abe.insert(45);
  abe.insert(28);
  abe.insert(13);
  abe.insert(17);
  abe.insert(14);
  abe.insert(34);
  abe.insert(21);
  abe.insert(33);
  abe.insert(42);
  abe.insert(43);
  abe.insert(26);
  abe.insert(12);
  abe.insert(21);
  abe.insert(23);

  std::cout << "Árbol Binario Equilibrado: \n";
  //mostrarPorNiveles(abe);
  std::cout << abe;  
  std::cout << std::endl;

  // busqueda de elementos
  std::cout << "Búsqueda de elementos: \n";
  std::cout << "Elemento 21: " << (abe.search(21) ? "Encontrado" : "No encontrado") << std::endl;
  std::cout << "Elemento 22: " << (abe.search(22) ? "Encontrado" : "No encontrado") << std::endl;

  // recorrido en inorden
  std::cout << "Recorrido en inorden: ";
  abe.inOrder(abe.getRoot());
  std::cout << std::endl;

  // recorrido en preorden
  std::cout << "Recorrido en preorden: ";
  abe.preOrder(abe.getRoot());
  std::cout << std::endl;

  // recorrido en postorden
  std::cout << "Recorrido en postorden: ";
  abe.postOrder(abe.getRoot());
  std::cout << std::endl;
}

void testABB(ABB<int> &abb)
{
  // 18 45 28 13 14 17 34 21 33 42 43 26 12 21 23
  abb.insert(18);
  abb.insert(45);
  abb.insert(28);
  abb.insert(13);
  abb.insert(14);
  abb.insert(17);
  abb.insert(34);
  abb.insert(21);
  abb.insert(33);
  abb.insert(42);
  abb.insert(43);
  abb.insert(26);
  abb.insert(12);
  abb.insert(21);
  abb.insert(23);

  std::cout << "Árbol Binario de Búsqueda: \n";
  //mostrarPorNiveles(abb);
  std::cout << abb;
  std::cout << std::endl;

  // busqueda de elementos
  std::cout << "Búsqueda de elementos: \n";
  std::cout << "Elemento 21: " << (abb.search(21) ? "Encontrado" : "No encontrado") << std::endl;
  std::cout << "Elemento 22: " << (abb.search(22) ? "Encontrado" : "No encontrado") << std::endl;

  // recorrido en inorden
  std::cout << "Recorrido en inorden: ";
  abb.inOrder(abb.getRoot());
  std::cout << std::endl;

  // recorrido en preorden
  std::cout << "Recorrido en preorden: ";
  abb.preOrder(abb.getRoot());
  std::cout << std::endl;

  // recorrido en postorden
  std::cout << "Recorrido en postorden: ";
  abb.postOrder(abb.getRoot());
  std::cout << std::endl;
}

int main()
{
  std::srand(std::time(nullptr));
  ABB<int> abb;
  ABE<int> abe;

  testABE(abe);
  testABB(abb);
  return 0;
}