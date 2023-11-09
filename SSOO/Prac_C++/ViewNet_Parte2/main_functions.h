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
 * @file main_functions.h 
 * @brief Donde se alojan las declaraciones de las funciones de los dos
 * programas anfitriones que usamos para enviar y recibir mensajes.
 *
 * @bug No hay bugs conocidos
 * 
 * Referencias: Enlaces de Interes
 * @see 
 * 
 * @version 1.0
 * @brief Historial de Revisiones 
 * 02/01/22 - Creacion (primera version) del codigo:
 *            He creado el archivo.
 * 02/01/22 - Archivo terminado.
 */

#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <system_error>

sockaddr_in make_ip_address(int port, const std::string& ip_address = std::string());