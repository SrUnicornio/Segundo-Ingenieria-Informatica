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
 * @file Socket.h 
 * @brief Donde se aloja la declaración de la clase Socket de nuestro programa.
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
 * 13/12/21 - Archivo terminado.
 */

#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <cstring>
#include <array>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

/**
 * @struct Message
 * @brief Estructura que almacena el mensaje que se enviara al programa que
 * recibe los mensajes.
 */
struct Message {
  std::array<char, 1024> text;    
};

/**
 * @class Socket
 * @brief Clase usada a Modo de Socket, podemos crearlos y usarlo.
 */
class Socket {
  public:
    /// Constructor.
    Socket(const sockaddr_in& address);
    /// Destructor.
    ~Socket();

    /// Funciones de la clase (Metodos).
    void send_to(const Message& message, const sockaddr_in& address);
    void receive_from(Message& message, sockaddr_in& address);
  
  private:
    int fd_; ///< Es el descriptor de archivo del socket que creará cada objeto de la clase
};
#endif