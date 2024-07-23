#ifndef __Libro_H__
#define __Libro_H__

#include "../includespath.h"

class Libro
{
public:
  // default constructor
  // isbn compuesto por 4 letras, 4 números y 1 letra de forma random
  Libro() : ISBN_(std::string(4, 'A' + rand() % 26) + std::to_string(rand() % 10000) + std::string(1, 'A' + rand() % 26)),
            titulo_("Default"), autor_("Default"), editorial_("Default"), fecha_("Default"), edicion_(0) {}
  Libro(const std::string &ISBN, const std::string &titulo, const std::string &autor, const std::string &editorial, const std::string &fecha, const int &edicion)
      : ISBN_(ISBN), titulo_(titulo), autor_(autor), editorial_(editorial), fecha_(fecha), edicion_(edicion) {}

  bool operator==(const Libro &other) const
  {
    return (ISBN_ == other.ISBN_);
  }

  bool operator!=(const Libro &other) const
  {
    return !(ISBN_ == other.ISBN_);
  }

  operator std::string() const { return ISBN_; }

  std::string getISBN() const { return ISBN_; }
  std::string getTitulo() const { return titulo_; }
  std::string getAutor() const { return autor_; }
  std::string getEditorial() const { return editorial_; }
  std::string getFecha() const { return fecha_; }
  unsigned getEdicion() const { return edicion_; }

  friend std::istream &operator>>(std::istream &is, Libro &libro)
  {
    std::cout << "\nEnter Libro ISBN (LLLLNNNNNL): ";
    is >> libro.ISBN_;
    std::cout << "Enter Libro titulo: ";
    is >> libro.titulo_;
    std::cout << "Enter Libro autor: ";
    is >> libro.autor_;
    std::cout << "Enter Libro editorial: ";
    is >> libro.editorial_;
    std::cout << "Enter Libro fecha (dd/mm/yyyy): "; // (dd/mm/yyyy
    is >> libro.fecha_;
    std::cout << "Enter Libro edicion: ";
    is >> libro.edicion_;
    return is;
  }

  friend std::ostream &operator<<(std::ostream &os, const Libro &libro)
  {
    os << libro.getISBN() << " " << libro.getTitulo() << " " << libro.getAutor() << " " << libro.getEditorial() << " " << libro.getFecha() << " " << libro.getEdicion();
    return os;
  }

private:
  // ISBN compuesto po LLLLNNNNNL : L=letra, N=número
  std::string ISBN_;
  std::string titulo_;
  std::string autor_;
  std::string editorial_;
  std::string fecha_;
  unsigned edicion_;

  bool ISBNValido(const std::string &ISBN)
  {
    if (ISBN.size() != 10)
    {
      return false;
    }
    for (int i = 0; i < 4; i++)
    {
      if (!isalpha(ISBN[i]))
      {
        return false;
      }
    }
    for (int i = 4; i < 9; i++)
    {
      if (!isdigit(ISBN[i]))
      {
        return false;
      }
    }
    if (!isalpha(ISBN[9]))
    {
      return false;
    }
    return true;
  }
};

#endif // __Libro_H__
