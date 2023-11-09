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
 * @file Socket.cc 
 * @brief Donde se aloja la implementacion de la clase Socket de nuestro
 * programa.
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

#include "Socket.h"

/**
 * @fn Socket::Socket(const sockaddr_in& address)
 * @brief Construye un nuevo objeto Socket.
 * 
 * @param address Usada para crear el objeto Socket.
 */
Socket::Socket(const sockaddr_in& address) : fd_(socket(AF_INET, SOCK_DGRAM, 0)) {
  if (fd_ < 0)
    throw std::system_error(errno, std::system_category(), "No se pudo crear el socket");

  if ((bind(fd_, reinterpret_cast<const sockaddr*>(&address), sizeof(address))) < 0)
    throw std::system_error(errno, std::system_category(), "Fallo en bind()");
}

/**
 * @fn Socket::~Socket()
 * @brief Destruye adecuadamente el objeto Socket creado.
 */
Socket::~Socket() {
  close(fd_);
}

/**
 * @fn void Socket::send_to(const Message& message, const sockaddr_in& address)
 * @brief Envia un mensaje al objeto Socket del programa que recibe mensajes.
 * 
 * @param message Struct tipo Message que contiene un array de carateres a ser enviado.
 * @param address Variable que es usada para dar cabida a la dirección IP que usaremos.
 */
void Socket::send_to(const Message& message, const sockaddr_in& address) {
  if (0 > sendto(fd_, &message, sizeof(message), 0, reinterpret_cast<const sockaddr*>(&address), sizeof(address)))
    throw std::system_error(errno, std::system_category(), "Fallo en send_to(), no se pudo enviar el mensaje");
}

/**
 * @fn void Socket::receive_from(Message& message, sockaddr_in& address)
 * @brief Recibe un mensaje de el objeto Socket del programa que envia mensajes.
 * 
 * @param message Variable tipo struct que contiene el mensaje a enviar.
 * @param address Direccion IP de quien envía el mensaje.
 */
void Socket::receive_from(Message& message, sockaddr_in& address) {
  socklen_t src_len{sizeof(address)}; ///< Guardamos el tamaño de la direccion a donde mandaremos el mensaje.
  if (0 > recvfrom(fd_, &message, sizeof(message), 0, reinterpret_cast <sockaddr*>(&address), &src_len))
    throw std::system_error(errno, std::system_category(), "Error recibiendo el mensaje.");

  message.text[1023] = '\0'; ///< Marcamos el final del mensaje por si acaso.
  char* remote_ip = inet_ntoa(address.sin_addr); ///< Convertimos la ip en un string.
  int remote_port = ntohs(address.sin_port); ///< Convertimos los puertos a enteros.
  std::cout << "El sistema " << remote_ip << ": " << remote_port;
  std::cout << "\nEnvió el mensaje:\n" << message.text.data() << "\n";
}