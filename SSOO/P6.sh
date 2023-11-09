#!/bin/bash
# Cristopher Manuel Afonso Mora - alu0101402031
# sysinfo - Un script que informa del estado del sistema

##### Constantes
TITLE="Información del sistema para $HOSTNAME"
RIGHT_NOW=$(date +"%x %r%Z")
TIME_STAMP="Actualizada el $RIGHT_NOW por $USER"

HELP="--help"
filename=~/sysinfo.txt
dofile=0
interactive=0


##### Estilos
TEXT_BOLD=$(tput bold)
TEXT_GREEN=$(tput setaf 2)
TEXT_RESET=$(tput sgr0)

##### Funciones
system_info() # El comando uname -a muestra toda la información del sistema
{
  echo -e "Función drive_space:"
  echo "${TEXT_ULINE}Versión del sistema${TEXT_RESET}"
  uname -a
}

show_uptime() # Muestra la salida del comando uptime
{
  echo -e "\nFunción show_uptime:"
  echo "${TEXT_ULINE}Tiempo de encendido del sistema${TEXT_RESET}"
  uptime
}

drive_space() # Actividad 1: Implementar la función drive_space
{
  echo -e "\nFunción drive_space: (Actividad 1)"
  echo -e "${TEXT_ULINE}Espacio ocupado en las Particiones/Discos duros del sistema:${TEXT_RESET}\n"
  df -a
}

home_space() # Actividad 2: Implementar la función home_space
{
  echo -e "\nFunción home_space: (Actividad 2)"
  echo "Usado   Directorio"
  if [ "$USER" == "root" ]; then
    du -s /home/*/ | sort -r
  else
    du -s /home/$USER
  fi
}

write_page() 
{
  cat << _EOF_
  $TEXT_BOLD$TITLE$TEXT_RESET
  $TEXT_GREEN$TIME_STAMP$TEXT_RESET

  $(system_info)
  $(show_uptime)
  $(drive_space)
  $(home_space)

_EOF_
}


main_message()
{
  echo "Modo de uso: $0 [-f filename] [-i] [-h]"
  echo "Pruebe '$0 $HELP' para más información"
}

error_message()
{
  echo "Este programa no permite más de 4 parámetros pasado a la vez"
  echo "Pruebe '$0 $HELP' para más información"
}

error_file() 
{
  echo "Para usar la opción -f hace falta un nombre válido de archivo"
  echo "Pruebe '$0 $HELP' para más información"
}

error_parameter()
{
  echo "Warning! el/los paramétros pasados a la función no son válidos"
  echo "Pruebe '$0 $HELP' para más información"
}

usage() # Función que explica el modo de uso del programa
{
  echo "Este script genera automáticamente un informe de estado del sistema"
  echo "y solo admite 3 opciones a la hora de ser usado."
  echo "Si usa '-h' ó '--help', no importa el resto de opciones que acompañen"
  echo "al programa, este mostrará un mensaje de ayuda (este mesaje) y se"
  echo "cerrará. Si usa '-i' ó '--interactive', entonces se abre la interfaz"
  echo "interactiva y elegirá lo que quiere hacer con el archivo sobre la"
  echo "marcha. Si usa '-f' ó '--file', entonces esta opción debe ir"
  echo "obligadamente acompañada de un nombre de archivo, porque ese nombre"
  echo "será el nombre que se le pondrá al archivo una vez generado,"
  echo "evidentemente, si usted pone por ejemplo '... -f -i ...'"
  echo "'-i' al ser una opción más del programa, no será aceptada y habrá"
  echo "un error. Las únicas opciones compatibles son '-i' | '--interactive',"
  echo "con '-f' | '--file', si se llaman a la vez, se abrirá la interfaz"
  echo "interactiva pero el nombre por defecto será el elegido por usted"
  echo "previamente al llamar a la opción '-f'. Antes de cerrar este mensaje"
  echo "de ayuda, le doy este recordatorio."
  echo "Modo de uso del programa: $0 [-f filename] | [-i] | [-h]"
}

interactive_interface() # Funcion de Actividad 4
{
  while [ true ]; do
    echo "Elija entre estas tres opciones:"
    echo "Opción 1: Solo mostrar el informe por pantalla y cerrar el programa"
    echo "Opción 2: Solo guardar el informe con el nombre elegido y cerrar el programa"
    echo "Opción 3: Cerrar el programa ahora mismo sin hacer nada" # Metí esta opción porque quise
    echo "Introduzca 1, 2 ó 3 para realizar su elección, cualquier otra opción será rechazada"
    read selection
    echo  ""
    case $selection in 
      1 )
        write_page
        break
        ;;
      2 )
        echo "Introduzca el nombre del archivo[$filename]:"
        read selection
        echo ""
        if [ "$selection" != "" ]; then
          filename=$selection
        fi
        if [ -f "$filename" ]; then
          while [ true ]; do
            echo "El archivo de destino existe. ¿Sobreescribir? (S/N)"
            read selection
            echo ""
            case $selection in
              S | s )
                write_page > $filename
                break
                ;;
              N | n )
                echo "No se ha efectuado ninguna tarea"
                echo "Fin del programa"
                break
                ;;
              * )
                echo -e "Por favor, eliga una opción válida\n"
            esac
          done
        else 
          write_page > $filename
        fi
        break
        ;;
      3 )
        echo "Fin del programa"
        break
        ;;
      * )
        echo -e "Opción inválida, intentelo de nuevo\n"
    esac
  done
}

##### Programa principal
# Filtros iniciales para que el programa no se cuelgue
if [ $# -eq 0 ]; then 
  main_message
  exit 0
fi

if [ $# -gt 4 ]; then
  error_message
  exit 1
fi

while [ "$1" != "" ]; do
  case $1 in
    -f | --file )
      dofile=1
      if [ "$2" != "" ]; then
        if [ "$2" != "-h" ]; then
          if [ "$2" != "-i" ]; then
            if [ "$2" != "--help" ]; then
              if [ "$2" != "--interactive" ]; then
                filename=$2
                shift
              else 
                error_file; exit 1
              fi
            else 
              error_file; exit 1
            fi
          else 
            error_file; exit 1
          fi
        else 
          error_file; exit 1
        fi
      else 
        error_file; exit 1
      fi
      ;;
    -i | --interactive )
      interactive=1
      ;;
    -h | --help )
      usage
      exit 0
      ;;
    * )
      error_parameter
      exit 1
      ;;
  esac
  shift
done

if [ $interactive -eq 1 ]; then
  interactive_interface
  exit 0
fi

if [ $dofile -eq 1 ]; then
  write_page > $filename
  exit 0
fi

error_parameter # Se supone que el programa nunca va a llegar
exit 1          # aquí, pero estas lineas están por si acaso

