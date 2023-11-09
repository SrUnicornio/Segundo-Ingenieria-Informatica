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
 * @file File.h 
 * @brief Donde se aloja la declaración de la clase File que usaran nuestros
 * archivos clientes.
 *
 * @bug No hay bugs conocidos
 * 
 * Referencias: Enlaces de Interes
 * @see 
 * 
 * @version 1.0
 * @brief Historial de Revisiones 
 * 12/12/21 - Creacion (primera version) del codigo:
 *            He creado el archivo.
 * 14/12/21 - Archivo terminado.
 */

#ifndef FILE_H  
#define FILE_H

#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 * @class File.
 * @brief Contiene el descriptor del archivo del mensaje ".txt" que usaremos
 * para enviarle el contenido del archivo al otro Socket.
 */
class File {
 public:
  /// Constructor.
  File(const char* pathname, int flags);

  /// Destructor.
  ~File();

  /// Funciones de la clase (Metodos).
  ssize_t Read(void* buf, size_t count);
  ssize_t Write(void* buf, size_t count);

 private:
  int fd_; ///< Descriptor de la operación open(archivo.c_str(), O_RDONLY);
};
#endif