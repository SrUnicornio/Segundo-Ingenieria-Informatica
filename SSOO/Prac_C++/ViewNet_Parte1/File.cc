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
 * @file File.cc 
 * @brief Donde se aloja la implementacion de la clase File
 * de nuestro programa contenida en File.h.
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

#include "File.h"

/**
 * @fn File::File(const char* pathname, int flags)
 * @brief Construye un nuevo objeto File, que contiene el descriptor de
 * archivo de un archivo de texto leido para ser enviado desde un programa
 * a otro.
 * 
 * @param pathname Es la ruta(relativa o absoluta) del archivo (su nombre).
 * @param flags Es el tamaño maximo que puede tener el mensaje, si este tiene
 * un tamaño superior, los bytes restantes se ignoraran.
 */
File::File(const char* pathname, int flags) : fd_(open(pathname, flags)) { 
  if (fd_ < 0) { ///< Si open() dio error, se termina el programa.
    std::cerr << "No se puedo abrir el fichero correctamente " 
              << std::strerror(errno) << std::endl;
    throw 1;
  }
}

/**
 * @fn File::~File()
 * @brief Destructor de la clase File, destruye el archivo del objeto File.
 */
File::~File() {
  close(fd_);
}

/**
 * @fn ssize_t File::Read(void* buf, size_t count)
 * @brief Método que abre el archivo y lee mensajes de "count" caracteres y
 * comprueba si ha habido algun fallo.
 * 
 * @param buf Contiene el mensaje a leer.
 * @param count Contiene el número entero que determina cual es el limite de
 * caracteres que se pueden leer del mensaje.
 * @return ssize_t Contiene el tamaño del mensaje en bytes.
 */
ssize_t File::Read(void* buf, size_t count) {
  ssize_t message_size{read(fd_, buf, count)};
  if (message_size < 0)
    throw std::system_error(errno, std::system_category(), "error al leer el fichero");

  return message_size;
}

/**
 * @fn ssize_t File::Write(void* buf, size_t count)
 * @brief Método que abre el archivo y escribe mensajes de "count" caracteres.
 * 
 * @param buf Contiene el mesaje a escribir.
 * @param count Contiene el número entero que determina cual es el limite de
 * caracteres que se pueden leer del mensaje.
 * @return ssize_t Contiene el tamaño del mensaje escrito en bytes.
 */
ssize_t File::Write(void* buf, size_t count) {
  return write(fd_, buf, count);
}