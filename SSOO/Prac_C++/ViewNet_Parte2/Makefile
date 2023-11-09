# Universidad de La Laguna (ULL)
# Escuela Superior de Ingenieria y Tecnologia (ESIT)
# Grado en Ingenieria Informatica
# Asignatura: Sistemas Operativos
# Curso: 2º
# Practica del Bloque de C++: Practica de Programacion
# @author Cristopher Manuel Afonso Mora
# Correo: alu0101402031@ull.edu.es
# @date 02/01/2022
# 
# @file Makefile
# @brief Archivo que ayuda a facilitar el proceso de compilacion de los dos
# programas anfitriones.
#
# @bug No hay bugs conocidos
# 
# Referencias: Enlaces de Interes
# @see 
# 
# @version 1.0
# @brief Historial de Revisiones 
# 02/01/22 - Creacion (primera version) del codigo:
#            Solo he creado el archivo.
# 02/01/22 - Archivo terminado.
#

CC = g++
CFLAGS = -g -I -std=c++17 -Wall -Weffc++ -Wextra 
DEPS = Socket.h main_functions.h File.h # En DEPS van los archivos .h
OBJ1 = main_send.o Socket.o File.o main_functions.o # En OBJ1 van todos los .cc pero con .o para compilar main_send.cc
OBJ2 = main_receive.o Socket.o File.o main_functions.o # En OBJ2 van todos los .cc pero con .o para compilar main_receive.cc

%.o: %.cc $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

recieve: $(OBJ2)
	$(CC) -o $@ $^ $(CFLAGS)

send: $(OBJ1)
	$(CC) -o $@ $^ $(CFLAGS)

clean: 
	rm -rf *.o
