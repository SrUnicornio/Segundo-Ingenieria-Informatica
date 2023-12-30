#!/bin/bash

# Práctica de BASH: Traza de llamadas del sistema en los procesos.
# Álvaro Pérez Ramos
# 10/13/2023

##### Constantes
TITLE="Traza de llamadas del sistema en los procesos. Por Álvaro Pérez Ramos"
SEPARADOR="========================================================================="

##### Estilos
TEXT_BOLD=$(tput bold)
TEXT_GREEN=$(tput setaf 2)
TEXT_RESET=$(tput sgr0)
TEXT_ULINE=$(tput sgr 0 1)

##### Variables
MY_PROG=""
PROGRAMS=()
OPCIONES_PROGG=()
OPCIONES_STO=""
BOOL_NATTCH=false
PID=""
BOOL_V=false

##### Funciones
error_exit() 
{
  echo "${TEXT_BOLD}${TEXT_GREEN}ERROR: ${TEXT_RESET}${TEXT_ULINE}${1}${TEXT_RESET}" 1>&2
  echo "Use $0 -h o --help para más información"
  exit 1
}

# La función usage() muestra la ayuda del programa.
usage ()
{
  # Muestra la ayuda del programa
  cat << _EOF_
  Sintaxis 'scdebug [-h | --help] [-sto arg] [-v | -vall] [-nattch progtoattach] [prog [arg1 ...]]'

  -h --help   Muestra la ayuda del programa.
  -sto arg    Opciones del strace pasado como argumento.
  -v --vall   Muestra la información de la llamada al sistema y el resultado de la llamada al sistema.
  -nattch     Monitorizar otros procesos que ya están en ejecución (modo attach), de forma que se pueda especificar el nombre del programa a monitorizar, y se opte por el proceso del usuario cuya ejecución se inició más recientemente con ese comando.
  PROG arg1   Programa a ejecutar scdebug, con sus argumentos.
  -rm        Elimina todo lo que contiene el directorio .scdebug/* o .scdebug/PROG/* en el caso de que se haya especificado un programa.
_EOF_
}

# Función que elimina el directorio .scdebug/$MY_PROG/*
Eliminar_Directorio() 
{
  if [ -d .scdebug ]; then
    rm -r .scdebug/$MY_PROG/*
  fi
}

# Función Get_PIDs() que obtiene los PID de los procesos que se están ejecutando
Get_PIDs()
{
  # Busca todos los PID que tengas $MY_PROG en su nombre 
  ps -u | ps -eo pid,cmd | grep $MY_PROG | grep "strace" |  grep -v grep |  tr -s " " 
  echo Ingrese el PID del proceso a monitorizar:
  read PID
  echo

}

# Función Correcto() comprobamos que los argumentos pasados son correctos
Correcto()
{
  if [ -x "$MY_PROG" ]; then
    echo "El programa $MY_PROG existe y es ejecutable"
    # Comprobamos si existe el directorio .scdebug/$MY_PROG
    if [ -n "$MY_PROG" ]; then
      # Carpeta en home del usuario
      mkdir -p "$HOME/.scdebug/$MY_PROG"
    fi
  else
    error_exit "El programa $MY_PROG no existe o no es ejecutable"
  fi
}

# Función Scdebug() que ejecuta strace con las opciones -sto y -nattch
Scdebug()
{
  # Comprobamos si se ha pasado la opción -nattch
  if [[ "$BOOL_NATTCH" == true ]]; then
    echo "Se ha pasado la opción -nattch"
    echo

    # Obtenemos los PIDs de los procesos que se están ejecutando
    Get_PIDs
    echo "Se va a monitorizar el proceso '$MY_PROG ${OPCIONES_PROGG[@]}' con PID $PID y las opciones '$OPCIONES_STO'"
    echo
    # Ejecutamos el strace con las opciones pasadas como argumento
    echo "strace $OPCIONES_STO -p $PID -o $HOME/.scdebug/$MY_PROG/trace_UUID.txt"
    strace $OPCIONES_STO -p $PID -o $HOME/.scdebug/$MY_PROG/trace_$(uuidgen).txt &
    echo
  else
    echo "No se ha pasado la opción -nattch"
    echo "Se va a monitorizar el proceso '$MY_PROG ${OPCIONES_PROGG[@]}' y las opciones '$OPCIONES_STO'"
    echo "Ejecutando strace con las opciones pasadas como argumento"
    echo

    # Ejecutamos el strace con las opciones pasadas como argumento
    echo "strace $OPCIONES_STO -o $HOME/.scdebug/$MY_PROG/trace_UUID.txt ./$MY_PROG ${OPCIONES_PROGG[@]}" 
    strace $OPCIONES_STO -o $HOME/.scdebug/$MY_PROG/trace_$(uuidgen).txt ./$MY_PROG "${OPCIONES_PROGG[@]}" &
    echo

    # Con el comando $! obtenemos el PID del último proceso en segundo plano
    echo "El PID del proceso en segundo plano es $!"
  fi
}

# Función Trazas() para consultar las trazas ya realizadas de un programa por -v o -vall
Trazas()
{
  # Comprobamos si se ha pasado la opción -v o -vall
  if [[ "$BOOL_V" == true ]]; then
    echo "Se ha pasado la opción -v y el programa $MY_PROG"
    echo
    echo "Se va a mostrar la última traza realizada del programa $MY_PROG en el directorio $HOME/.scdebug/$MY_PROG"
    echo

    # Comprobamos si existe el directorio $HOME/.scdebug/$MY_PROG
    if [ -d $HOME/.scdebug/$MY_PROG ]; then
        # Obtenemos el archivo con fecha de modificación más reciente
        ARCHIVO=$(ls -t $HOME/.scdebug/$MY_PROG | head -n1)

        # Mostramos el contenido del archivo
        echo "============== COMMAND: $MY_PROG ======================="
        echo "============== TRACE FILE: $ARCHIVO ================="
        echo "============== TIME: $(stat -c %y $HOME/.scdebug/$MY_PROG/$ARCHIVO) =============="
    
    else
        error_exit "No existe el directorio .scdebug/$MY_PROG"
    fi
  else
    echo "Se ha pasado la opción -vall y el programa $MY_PROG"
    echo
    echo "Se van a mostrar todas las trazas realizadas del programa $MY_PROG en el directorio $HOME/.scdebug/$MY_PROG"
    echo

    # Comprobamos si existe el directorio $HOME/.scdebug/$MY_PROG
    if [ -d $HOME/.scdebug/$MY_PROG ]; then
      # Obtenemos los archivos del directorio $HOME/.scdebug/$MY_PROG ordenados de más reciente a más antiguo
      ARCHIVOS=($(ls -t $HOME/.scdebug/$MY_PROG))

      # Mostramos el contenido de cada archivo
      for ARCHIVO in "${ARCHIVOS[@]}"; do
        echo "============== COMMAND: $MY_PROG ======================="
        echo "============== TRACE FILE: $ARCHIVO ================="
        echo "============== TIME: $(stat -c %y $HOME/.scdebug/$MY_PROG/$ARCHIVO) =============="
      done
    else
      error_exit "No existe el directorio .scdebug/$MY_PROG"
    fi
  fi
  exit 0
}

##### Programa principal
cat << _EOF_
$SEPARADOR
$TITLE
$SEPARADOR
_EOF_

# Comprobamos que se haya pasado algún argumento
if [ $# -eq 0 ]; then
  error_exit "Faltan argumentos"
fi

while [ "$1" != "" ]; do
  case "$1" in
    -rm)
      Eliminar_Directorio
      exit 0
      ;;
    -k)
      shift
      
      ;;
    -h | --help)
      usage
      exit 0
      ;;
    -v | -vall)
      if [[ "$1" == "-v" ]]; then
        BOOL_V=true
        
      else if [[ "$1" == "-vall" ]]; then
        BOOL_V=false
      else if [[ "$2" == "-*" ]]; then
          error_exit "-v -vall no admite argumentos adicionales $1"
          exit 1
          fi
        fi
      fi
      shift
      MY_PROG=$1

      Trazas
      shift
      ;;
    -sto)
      shift
      if [ -n "$1" ]; then
        OPCIONES_STO=$1
        shift
      else
        error_exit "No se ha pasado ningún argumento para la opción -sto"
      fi
      ;;
    -nattch)
      BOOL_NATTCH=true
      shift
        if [[ "$1" == "-*" ]]; then
          error_exit "$1 no es un programa válido"
          exit 1
        fi
        PROGRAMS+=("$1")
        shift
      ;;
    *)
      MY_PROG=$1
      shift
      
      while [ "$1" != "" ]; do
        if [[ "$1" == "-*" ]]; then
          error_exit "$1 no es un arg válido"
          exit 1
        fi
        OPCIONES_PROGG+=("$1")
        shift
      done
      ;;
  esac
done

Correcto
Scdebug

exit 0

