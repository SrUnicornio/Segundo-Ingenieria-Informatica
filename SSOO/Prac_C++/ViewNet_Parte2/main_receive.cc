/** 
 * Universidad de La Laguna (ULL)
 * Escuela Superior de Ingenieria y Tecnologia (ESIT)
 * Grado en Ingenieria Informatica
 * Asignatura: Sistemas Operativos
 * Curso: 2º
 * Practica del Bloque de C++: Practica de Programacion
 * @author Cristopher Manuel Afonso Mora
 * Correo: alu0101402031@ull.edu.es
 * @date 02/01/2022
 * 
 * @file main_receive.cc 
 * @brief Programa cliente encagado de recibir los mensajes enviados por el
 * objeto Socket creado en el archivo cliente "main_send.cc".
 *
 * @bug No hay bugs conocidos
 * 
 * Referencias: Enlaces de Interes
 * @see 
 * 
 * @version 1.0
 * @brief Historial de Revisiones 
 * 02/01/22 - Creacion (primera version) del codigo:
 *            Solo he creado el archivo.
 * 02/01/22 - Archivo terminado.
 */

#include "main_functions.h"
#include "Socket.h"
#include "File.h"

/**
 * @fn int protected_main()
 * @brief Funcion que suplanta al main() a modo de respaldo de seguridad, de
 * forma que si hay algun error de algun tipo al ejecutar la funcion 
 * protected_main(), el main() sabra manejar bien este error y no sera un error
 * que continue aun cuando el programa finalice su ejecucion.
 * 
 * @return int Valor que informa si la funcion se ha ejecutado correctamente
 * o si ha sufrido un error en algun lado.
 */
int protected_main() {
  sockaddr_in dir_remote{make_ip_address(13001, "127.0.0.1")};
  Socket socket_remoto(make_ip_address(12001, "127.0.0.1"));

  /// Recibimos el mensaje en message.
  Message message{};
  std::cout << "El programa esta esperando un mensaje\n";
  socket_remoto.receive_from(message, dir_remote);

  return 0;
}

int main() {
  try {
    return protected_main();
  } 
  catch (...) {
    std::cerr << "Ha habido un error." << std::endl;
    return 99;
  }
}