#! /bin/bash

# Cristopher Manuel Afonso Mora 
# alu0101402031
# Entregable Práctica 3 de SSOO

# Nota importante 1, yo tengo el ordenador en inglés, por eso el apartado
# 2 y 4 las palabras que se buscan con grep están en ingles
# Si su ordenador está en español, con cambiar las cadenas a su traduccion 
# En español sería suficiente

# Nota importante 2, para que este script funcione, debe ser ejecutado así:     source ./entregable3.sh
# Si no se ejecuta así, aunque los comandos están bien, no se ejecutan
# adecuadamente al ejecutar el este script

# Apartado 1: 
# Contar el número de proceso de nuestra sesión que están ejecutándose en segundo plano
echo -e "\nApartado 1 - Numero de procesos en segundo plano nuestros: "
jobs | wc -l     # Apartado 1 Solucion

# Apartado 2: 
# Contar el número de procesos que están en estado detenidos de entre los de nuestra sesión
echo -e "\nApartado 2 - Numero de procesos nuestros en estado detenido: "
jobs | grep "Stopped" | wc -l   # Apartado 2 Solucion

# Apartado 3: 
# Mostrar el nombre de los procesos que se están ejecutando en segundo plano
echo -e "\nApartado 3 - Estos son los procesos que se están ejecutando en segundo plano: "
jobs | awk '{print $3}'  # Apartado 3 Solucion

# Apartado 4
# Contar el número total de procesos que estamos ejecutando
echo -e "\nApartado 4 - Numero de procesos que estamos ejecutando: "
jobs | grep "Running" | wc -l   # Apartado 4 Solucion

# Apartado 5
# Sacar un listado de todos los directorios que hay en nuestro directorio actual
echo -e "\nApartado 5 - Lista de todos los directorios que hay en nuestro directorio actual"
ls -ld */   # Apartado 5 Solucion

# Apartado 6
# Contar el número de programas alojados en nuestro directorio actual 
# donde el grupo otros tiene permisos de ejecución
echo -e "\nApartado 6 - Contar el numero de programas en nuestro directorio actual"
echo -e "donde el grupo 'otros' tiene permisos de ejecucion: "
ls -l | awk '{print $1}' | grep "x$" | wc -l    # Apartado 6 Solucion
echo -e ""
