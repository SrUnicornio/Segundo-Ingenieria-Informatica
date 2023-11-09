#! /bin/bash

# Cristopher Manuel Afonso Mora 
# alu0101402031
# Entregable Práctica 3 de SSOO

# Ejercicio
# Contenta los siguientes apartados en un archivo de texto de tipo script bash.
# Las cuestiones las puedes contestar
# después de la línea de código bash en un comentario

# a) Crea una estructura de directorios como: 
#./temp
#./temp/tempa
#./temp/tempa/temp2
#./temp/tempa/temp1
#./temp/tempb
#./temp/tempb/temp2
#./temp/tempb/temp1

#con el commando: mkdir -p temp/temp{a,b}/temp{1,2}

mkdir -p temp/temp{a,b}/temp{1,2}  # Linea de comandos

#Cuestión: ¿Qué tipos de expansiones se están realizando?
#Respuesta: Se está realizando una expansión de llaves "{ }"






#b) Usa el comando find: find  ~ -name ‘temp*’ -type d

#para mostrar las rutas de los directorios creados en el paso 1 y usa una tubería y grep para
#seleccionar solo las carpetas finales que terminan en 1 y finalmente otra tubería con xargs y
#chmod para retirar el permiso de lectura de los directorios en el grupo otros.

# Respuesta al planteamiento de arriba: find  ~ -name 'temp*' -type d | grep "1$" | xargs chmod -v o-r

#Ayuda Ejemplo de xargs

# echo rojo | xargs echo azul
# produce como salida
# azul rojo

# echo rojo | xargs -I'{}' echo azul '{}' verde
# produce como salida:
# azul rojo verde 

# porque la opción -I'{}' permite definir la cadena '{}' como punto donde se sustituirá 
# cada línea de la entrada estándar que llega a xargs antes de ejecutar el comando.

# Cuestión: ¿Por qué crees que es necesario el entrecomillado en el argumento que sigue a la opción name del comando find?
# Respuesta: el entrecomillado es para que el comando find coja la cadena tal cual, literal sin modificar, y busque en 
#            todas las carpetas, ese patrón de caracteres a la hora de buscar en todos los archivos dentro de la carpeta del usuario






# c) Trata de escribir una línea de comando que permite automatizar la tarea de copia de un archivo
# a todas las carpetas de la estructura de directorios creada en el apartado 1 cuyo nombre termina en '1'.
# (Opcional) 

