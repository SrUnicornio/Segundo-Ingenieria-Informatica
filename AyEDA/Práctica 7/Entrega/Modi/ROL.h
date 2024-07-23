/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * 
 * Algoritmos y Estructuras de Datos
 * Práctica 5: Algoritmos de ordenación
 * 
 * @file ROL.h
 * @author Álvaro Pérez Ramos
 * @date 02 MAR 2024
 * @brief Fichero de cabecera para la clase ROL
*/

#ifndef __ROL_H__
#define __ROL_H__

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

static int ID = 0;
class ROL
{
public:
  // criar un personaje de rol con stats aleatorias
  ROL() : nombre_("Jugador" + std::to_string(ID++)), fuerza_(rand() % 20 + 1), destreza_(rand() % 20 + 1), constitucion_(rand() % 20 + 1), inteligencia_(rand() % 20 + 1), sabiduria_(rand() % 20 + 1), carisma_(rand() % 20 + 1) 
  {
    // meter al jugador en el vector de jugadores
    playersNames.push_back(nombre_);
  }
  ROL(std::string nombre, int fuerza, int destreza, int constitucion, int inteligencia, int sabiduria, int carisma) : nombre_(nombre), fuerza_(fuerza), destreza_(destreza), constitucion_(constitucion), inteligencia_(inteligencia), sabiduria_(sabiduria), carisma_(carisma) 
  {
    ID++;
    // meter al jugador en el vector de jugadores
    playersNames.push_back(nombre_);
  }
   // destructor
  ~ROL() {}

  // getters
  std::string getNombre() const { return nombre_; }
  int getFuerza() const { return fuerza_; }
  int getDestreza() const { return destreza_; }
  int getConstitucion() const { return constitucion_; }
  int getInteligencia() const { return inteligencia_; }
  int getSabiduria() const { return sabiduria_; }
  int getCarisma() const { return carisma_; }

  // setters
  void setNombre(std::string nombre) { nombre_ = nombre; }
  void setFuerza(int fuerza) { fuerza_ = fuerza; }
  void setDestreza(int destreza) { destreza_ = destreza; }
  void setConstitucion(int constitucion) { constitucion_ = constitucion; }
  void setInteligencia(int inteligencia) { inteligencia_ = inteligencia; }
  void setSabiduria(int sabiduria) { sabiduria_ = sabiduria; }
  void setCarisma(int carisma) { carisma_ = carisma; }

  // sobrecarga de operadores
  friend std::ostream &operator<<(std::ostream &out, const ROL &rol)
  {
    out << "Nombre: " << rol.nombre_ << std::endl;
    out << "Fuerza: " << rol.fuerza_ << std::endl;
    out << "Destreza: " << rol.destreza_ << std::endl;
    out << "Constitución: " << rol.constitucion_ << std::endl;
    out << "Inteligencia: " << rol.inteligencia_ << std::endl;
    out << "Sabiduría: " << rol.sabiduria_ << std::endl;
    out << "Carisma: " << rol.carisma_ << std::endl;
    return out;
  }

  friend std::istream &operator>>(std::istream &in, ROL &rol)
  {
    std::cout << "Nombre: ";
    in >> rol.nombre_;
    // si el nombre ya existe, no se modifica las stats y se procede a pedir la media de las stats
    for (int i = 0; i < playersNames.size(); i++)
    {
      if (rol.nombre_ == playersNames[i])
      {
        std::cout << "El nombre ya existe, no se puede modificar las stats" << std::endl;
        in >> rol.mediaStas_;
        return in;
      }
    }
    std::cout << "Fuerza: ";
    in >> rol.fuerza_;
    std::cout << "Destreza: ";  
    in >> rol.destreza_;
    std::cout << "Constitución: ";
    in >> rol.constitucion_;
    std::cout << "Inteligencia: ";
    in >> rol.inteligencia_;
    std::cout << "Sabiduría: ";
    in >> rol.sabiduria_;
    std::cout << "Carisma: ";
    in >> rol.carisma_;
  
    return in;
  }

  bool operator<(const ROL &rol) const
  {
    return (calcularMediaStas() < rol.calcularMediaStas() && getNombre() < rol.getNombre());
  }

  bool operator>(const ROL &rol) const
  {
    return (calcularMediaStas() > rol.calcularMediaStas() && getNombre() > rol.getNombre());
  }

  bool operator==(const ROL &rol) const
  {
    return (calcularMediaStas() == rol.calcularMediaStas() && getNombre() == rol.getNombre());
  }

  // funciones
  double calcularMediaStas() const 
  {
    return (getFuerza() + getDestreza() + getConstitucion() + getInteligencia() + getSabiduria() + getCarisma()) / 6;
  }
private:
  std::string nombre_;
  int fuerza_;
  int destreza_;
  int constitucion_;
  int inteligencia_;
  int sabiduria_;
  int carisma_;

  double mediaStas_ = calcularMediaStas();

  static std::vector<std::string> playersNames;
};

std::vector<std::string> ROL::playersNames;

#endif // __ROL_H__