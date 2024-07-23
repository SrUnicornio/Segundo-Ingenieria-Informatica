#ifndef __NODOB_H__
#define __NODOB_H__

// Clase NodoB (Nodo Binario)
template <typename Key>
class NodoB
{
private:
  Key data_;     // Contain the data of the node
  NodoB *left_;  // Pointer to the left child
  NodoB *right_; // Pointer to the right child

public:
  // Constructor
  NodoB(const Key &data, NodoB *left = nullptr, NodoB *right = nullptr) : data_(data), left_(left), right_(right) {}
  // Destructor
  ~NodoB()
  {
    delete left_;
    delete right_;
  }

  // Getters
  const Key &getData() const { return data_; }
  NodoB *getleft() const { return left_; }
  NodoB *getright() const { return right_; }

  // Setters
  void setData(Key data) { data_ = data; }
  void setleft(NodoB *left) { left_ = left; }
  void setright(NodoB *right) { right_ = right; }
};

#endif // __NODOB_H__