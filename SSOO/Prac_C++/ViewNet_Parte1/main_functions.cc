/** 
 * Universidad de La Laguna (ULL)
 * Escuela Superior de Ingenieria y Tecnologia (ESIT)
 * Grado en Ingenieria Informatica
 * Asignatura: Sistemas Operativos
 * Curso: 2º
 * Practica del Bloque de C++: Practica de Programacion
 * @author Cristopher Manuel Afonso Mora
 * Correo: alu0101402031@ull.edu.es
 * @date 12/12/2021
 * 
 * @file main_functions.cc 
 * @brief Donde se aloja la implementacion de las funciones de los dos
 * programas cliente que usamos para enviar y recibir mensajes.
 *
 * @bug No hay bugs conocidos
 * 
 * Referencias: Enlaces de Interes
 * @see 
 * 
 * @version 1.0
 * @brief Historial de Revisiones 
 * 12/12/21 - Creacion (primera version) del codigo:
 *            Solo he creado el archivo.
 * 14/12/21 - Archivo terminado.
 */

#include "main_functions.h"

/**
 * @fn sockaddr_in make_ip_address(int port, const std::string& ip_address)
 * @brief Función que asigna nuevas direcciones IP al Socket.
 * 
 * @param port Puerto que recibe la función, es un entero.
 * @param ip_address Direccion IP que recibe la funcion que es un std::string.
 * @return sockaddr_in es un struct generica que da cabida a cualquier tipo de direccion de cualquier tecnologia.
 */
sockaddr_in make_ip_address(int port, const std::string& ip_address) {
  sockaddr_in new_address{}; ///< Dirección que retornaremos
  new_address.sin_family = AF_INET;  ///< El dominio del socket será TCP/IP
  new_address.sin_port = htons(port); ///< Asignamos el puerto con htons y lo convertimos a 16 bits

  /// sin_addr es la IP y s_addr es la IP en bytes
  /// Si no se pasa nada se asigna por defecto INADDR_ANY y con htonl lo convierte en 32 bits
  if (ip_address.empty()) new_address.sin_addr.s_addr = htonl(INADDR_ANY);
  else inet_aton(ip_address.c_str(), &new_address.sin_addr); ///< Con c_str el string(IP) paramos a char*, y este con inet_aton lo conviertes en 32 bits

  return new_address;
}
