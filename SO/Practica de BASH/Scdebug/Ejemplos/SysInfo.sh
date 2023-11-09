# !/bin/bash

# sysinfo - Un script que informa del estado del sistema

##### Constantes

TITLE="Información del sistema para $HOSTNAME"
RIGHT_NOW=$(date +"%x %r%Z")
TIME_STAMP="Actualizada el $RIGHT_NOW por $USER"
SEPARADOR_="========================================================================="

##### Estilos
TEXT_BOLD=$(tput bold)
TEXT_GREEN=$(tput setaf 2)
TEXT_RESET=$(tput sgr0)
TEXT_ULINE=$(tput sgr 0 1)

##### Funciones

usage()
{
  echo "Ayuda"
}

system_info()
{
  # Función de stub temporal
  echo "$SEPARADOR_"
  echo "${TEXT_BOLD}Función system_info${TEXT_RESET}"
  echo "$SEPARADOR_"
  echo "${TEXT_ULINE}Información del sistema${TEXT_RESET}"
  echo
  uname -a
  echo "$SEPARADOR_"
}


show_uptime()
{
  # Función de stub temporal
  echo "$SEPARADOR_"
  echo "${TEXT_BOLD}Función show_uptime~${TEXT_RESET}"
  echo "$SEPARADOR_"
  echo "${TEXT_ULINE}Tiempo de encendido del sistema${TEXT_RESET}"
  echo
  uptime
  echo "$SEPARADOR_"
}


drive_space() 
{
  # Función de stub temporal
  echo "$SEPARADOR_"
  echo "${TEXT_BOLD}Función drive_space${TEXT_RESET}"
  echo "$SEPARADOR_"
  echo "${TEXT_ULINE}Espacio en disco${TEXT_RESET}"
  echo
  # Espacio usado en el sistema de archivos
  df -h 
  echo "$SEPARADOR_"
}


home_space() 
{
  # Función de stub temporal
  echo "$SEPARADOR_"
  echo "Función home_space"
  echo "$SEPARADOR_"
  echo "${TEXT_ULINE}Espacio en el directorio personal${TEXT_RESET}"
  echo
  if [ "$USER" = root ]; then
    echo "Espacio ocupado por cada uno de los subdirectorios en /home"
    echo
    du -sh /home/* | sort -nr
    echo "$SEPARADOR_"
    echo
  else
    echo "Espacio ocupado por el directorio personal del usuario"
    echo
    du -sh $HOME | sort -nr
    echo "$SEPARADOR_"
  fi
}

var_stats () 
{
  # Mostar un encavezado "Estadisticas en /var" 
  echo "$SEPARADOR_"
  echo "${TEXT_BOLD}Función var_stats${TEXT_RESET}"
  echo "$SEPARADOR_"
  echo "${TEXT_ULINE}Estadisticas en /var${TEXT_RESET}"
  echo
  # Comprobar si el directorio /var existe y es un directorio
  # si es así mostrar
  #   - Número total de archivos en /var
  #   - Número total de directorios en /var
  if [ -d /var ]; then
    echo "Número total de archivos en /var: $(find /var -type f 2> /dev/null | wc -l)"
    echo "$SEPARADOR_"
    echo
    echo "Número total de directorios en /var: $(find /var -type d 2> /dev/null | wc -l)"
    echo "$SEPARADOR_"
    echo
  else
    echo "El directorio /var no existe"
    echo "$SEPARADOR_"
    echo
  fi
}

##### Programa principal
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

show_help=0
interactive=0
filename=

while [ "$1" != "" ]; do
  case $1 in
    -f | --file )
      if [[ "$2" == "-*" ]]; then
        echo "Opción $1 requiere un argumento"
        exit 1
      fi
      filename=$2
      shift
      ;;
    -i | --interactive )
      interactive=1
      ;;
    -h | --help ) 
      show_help=1
      ;;
    * )
      usage
      exit 1
    esac
    shift
done

echo "show_help: $show_help"
echo "interactive: $interactive"
echo "filename: $filename"


if [ $show_help = 1 ]; then
  usage
  exit 0
fi

write_page

exit 0