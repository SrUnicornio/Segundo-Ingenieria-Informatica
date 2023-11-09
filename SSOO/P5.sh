#!/bin/bash

# Actividad Miércoles 27 de octubre 9:30

# Ejercicio
# Crea un script bash donde los siguientes apartados se resuelvan mediante funciones. 
# Además incluye el código necesario para llamar a cada una de las funciones y mostrar los resultados.

# a) Crea una función que devuelva los tipos de los sistemas de archivos de los dispositivos montados,
#    cuyo directorio de montaje no esté en '/snap'. No debe devolver repeticiones.

fn_fstypes() #creo la funcion que se nos pide en el apartado a
{
  echo "Primera funcion"
  df --output=fstype,size,target | tail -n+1 | grep -v "/snap" | tr -s ' ' | sort | cut -d ' ' -f1 | uniq
}

# b)  Crea una función que devuelva el total de espacio de los dispositivos montados cuyo tipo de sistema de archivos es tmpfs.
#     Esta función se puede resolver de varias maneras como por ejemplo apoyándose en la utilidad bc (cuando está instalada), 
#     el filtro awk o mediante comandos propios de bash. Trataremos de hacerlo aquí de esta última forma.

fn_total() #creo la funcion que se nos pide en el apartado b
{
  echo "Segunda funcion"
  df -k | tail -n+2 | grep "tmpfs" | tr -s ' ' | cut -d ' ' -f2 | while read; do let count=count+REPLY;echo $count; done | tail -n1
}

#funcion principal
echo -e "\nApartado a): Llamada de la primera funcion\n"
fn_fstypes #llamo a la funcion del apartado a)
echo -e "\n"
echo -e "Apartado b): Llamada de la segunda funcion\n"
fn_total #llamo a la funcion del apartado b)
echo -e "\n"