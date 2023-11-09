#!/bin/bash 
# Cristopher Manuel Afonso Mora - alu0101402031
# userProc - Un script que muestra los usuarios que tienen al menos un proceso
# con tiempo de CPU consumido mayor a N, donde si no se pasa ningún parámetro
# N será 1 segundo, con el parámetro -t N indicaremos el tiempo a partir del
# que se mostrarán los procesos.

##### Constantes
TITLE="Programa: userProc          Creador: Cristopher Manuel Afonso Mora"

##### Varaibles
help="--help"             # Objetivo 1 Funcionamiento básico
n=1                       # Objetivo 1 Funcionamiento básico, var. int, tiempo en segundos que nos dice el valor para filtrar los usuarios
t=0                       # Objetivo 1 Funcionamiento básico, var. booleano, nos dice si se ha llamado al parámetro '-t'
usr=0                     # Objetivo 2 Solo usuarios con conexion, var. booleano, nos dice si se ha llamado al parámetro '-usr'
u=0                       # Objetivo 3 Filtra por usuario, nos dice si se ha llamado al parámetro '-u'
var_u_activated=0         # Objetivo 3 Filtra por usuario, nos dice si se ha llamado al parámetro '-u', igual a la anterior pero se trata diferente, nos sirve para no hacer shift del valor que esté a la derecha del último nombre del usuario
list_defined_by_user=     # Objetivo 3 Filtra por usuario, lista que contiene a los usuarios dados por el usuario, esta lista cuando está comprobada que los usuarios están bien, se la pasamos a la función
count=0                   # Objetivo 4 Número de procesos por usuario, var. booleano, nos dice si se ha llamado al parámetro '-count'
inv=0                     # Objetivo 5 Cambios en la ordenacion, var. booleano, nos dice si se ha llamado al parámetro '-inv'
pid=0                     # Objetivo 5 Cambios en la ordenacion, var. booleano, nos dice si se ha llamado al parámetro '-pid'
c=0                       # Objetivo 5 Cambios en la ordenacion, var. booleano, nos dice si se ha llamado al parámetro '-c'
kind_of_sort="sort -k 1"  # Objetivo 5 Cambios en la ordenación, var. con comando, nos dice que comando de ordenación debemos aplicarle a la tabla al final para mostrarle acorde a las peticiones del usuario, por defecto, ordenamos por la columna 1 (usuarios)

##### Estilos
TEXT_BOLD=$(tput bold)
TEXT_RED=$(tput setaf 1)
TEXT_GREEN=$(tput setaf 2)
TEXT_BLUE=$(tput setaf 4)
TEXT_PINK=$(tput setaf 5)
TEXT_WATER=$(tput setaf 6)
TEXT_RESET=$(tput sgr0)
TEXT_ULINE=$(tput sgr 0 1)

##### Funciones
users_table() # Objetivo 1 Funcionamiento basico
{
  if [ "$usr" == "1" ]; then # Objetivo 2 Solo usuarios con conexión
    list_users=$(who | cut -d ' ' -f1 | sort)
  elif [ "$u" == "1" ]; then
    list_users=$(echo "$list_defined_by_user" | tr -s ' ' '\n' | sort)
  else 
    list_users=$(ps --no-headers -eo user:16 | sort | uniq)
  fi

  if [ "$inv" == "1" ]; then # Objetivo 5 Cambios en la ordenación
    kind_of_sort="sort -rk 1"
  fi

  if [ "$pid" == "1" ]; then
    if [ "$inv" == "1" ]; then
      kind_of_sort="sort -nrk 5"
    else
      kind_of_sort="sort -nk 5"
    fi
  fi

  if [ "$c" == "1" ]; then
    if [ "$inv" == "1" ]; then
      kind_of_sort="sort -nrk 4"
    else
      kind_of_sort="sort -nk 4"
    fi
  fi

  echo "${TEXT_RED}Usuario           GID    UID    Nº Procesos    PID    Tiempo${TEXT_RESET}"

  for user in $list_users; do # Objetivo 1 Funcionamiento basico
    time_id_proces_user=$(ps --no-headers -u $user -o time,pid --sort=-time | head -n1 | awk -F ':' '{printf 3600*$1+60*$2+$3}')

    # si el proceso con mayor tiempo consumido por el usuario, es menor al parámetro dado por el usuario, entonces no se muestra    
    if [ "$time_id_proces_user" -gt "$n" ]; then 
      id_user=$(id -u $user)
      id_ugroup=$(id -g $user)
      if [ "$count" == "1" ]; then # Objetivo 4 Numero de procesos de usuario
        for i in $(ps -u $user --no-headers -o time | sort); do
          if [ "$(echo "$i" | awk -F ':' '{printf 3600*$1+60*$2+$3}')" -gt "$n" ]; then # si el número es mayor, se suma 1 al contador de procesos
            num_proces=$((${num_proces}+1))
          fi
        done
      else
        num_proces=$(ps -u $user --no-headers | wc -l)
      fi

      id_proces_user=$(ps --no-headers -u $user -o pid --sort=-time | head -n1)
      printf "${TEXT_WATER}%-18s${TEXT_PINK}%-7s%-7s%-15s%-7s%s\n" $user $id_ugroup $id_user $num_proces $id_proces_user $time_id_proces_user
    fi

  done | $kind_of_sort # El resultado del while se va acumulando como un subproceso, a la espera de ser ordenado por el comando elegido para ordenar
}

usage() # Objetivo 1 Funcionamiento basico, mensaje de ayuda
{
  echo "$TITLE"
  echo ""
  echo "Este programa muestra un listado con todos los usuarios que tienen al"
  echo "menos un proceso con un tiempo de CPU consumido mayor a N, donde si no"
  echo "se pasa ningún parámetro, N será 1 segundo (N está en segundos)."
  echo ""
  echo "Modo de Uso:    $0 [-t N] || [-h | $help] || [-u NAMES | -usr] || [-count] || [-inv] || [-pid | -c]"
  echo ""
  echo "-h     ==> palabra clave para solicitar instrucciones de uso (si se usa esta opción, solo se muestra este mensaje de ayuda)"
  echo "$help ==> Equivalente al parámetro \"-h\", es su opción extendida"
  echo "-t     ==> Ajusta el tiempo a partir del cual se muestran los procesos"
  echo " N     ==> El tiempo a partir del cual se muestran los procesos"
  echo "-u     ==> Le decimos al programa el nombre concreto de todos los usuarios que queremos evaluar (opción incompatible con \"-usr\")"
  echo " NAMES ==> Son los nombres de todos los usuarios que queremos evaluar"
  echo "-usr   ==> Le decimos al programa que queremos que solo nos evalue a los usuarios que están actualmente conectados al sistema (opción incompatible con \"-u\")"
  echo "-count ==> Le decimos al programa que queremos que en la tabla solo se vean los procesos que cumplen la condición de ser mayor a N (al N que acompaña al parámetro \"-t\")"
  echo "-inv   ==> Le decimos al programa que nos ordene la tabla de forma inversa (ya sea inversa por las columnas de usuario, del PID, o del Nº de Procesos)"
  echo "-pid   ==> Le decimos al programa que nos ordene la tabla por la columna del PID (opción incompatible con \"-c\")"
  echo "-c     ==> Le decimos al programa que nos ordene la tabla por la columna del Nº de Procesos (opción incompatible con \"-pid\")"
}

error_option_t_value_no_positive() # Objetivo 1 Funcionamiento basico, mensaje de error si -t no recibe un número positivo
{
  echo "Warning! no se ha dado un número positivo al llamar al parámetro '-t'"
  echo "Ejecución abortada. Pruebe '$0 $help' para más información."
}

error_parameter_wrong() # Objetivo 1 Funcionamiento basico, mensaje de error si le pasamos al programa un parámetro que no acepta
{
  echo "Warning! se ha llamado al programa con un parámetro desconocido"
  echo "Ejecución abortada. Pruebe '$0 $help' para más información."
}

error_sort() # Objetivo 5 Cambios en la ordenacion, mensaje de error que se muestra si se ponen a la vez los comandos '-pid' y '-c', ya que son incompatibles
{
  echo "Warning! se ha invocado a la función con los parámetros '-pid' y '-c'"
  echo "Estos parámetros son incompatibles, vuelva a llamar a la funcion sin"
  echo "Algunos de ellos. Ejecución abortada."
  echo "Ejecución abortada. Pruebe '$0 $help' para más información."
}

error_parameter_duplicated() # Objetivo 1 Funcionamiento basico, mensaje de error que se muestra si llamamos a un mismo parámetro más de una vez
{
  echo "Warning! se ha invocado a un parámetro más de una vez. Ejecución abortada"
  echo "Pruebe '$0 $help' para más información."
}

error_usr_u() # Objetivo 3 Filtra por usuario, mensaje de error que se muestra si llamamos a los parámetros '-u' y '-usr' a la vez, ya que son incompatibles
{
  echo "Warning! se ha invocado al parámetro '-u' y '-usr' a la vez. Ejecución abortada"
  echo "Pruebe '$0 $help' para más información."
}

error_u_no_users() # Objetivo 3 Filtra por usuario, mensaje de error que se muestra si llamamos a '-u' sin un solo usuario
{
  echo "Warning! se ha llamado al parámetro '-u' sin nombres de usuarios. Ejecución abortada"
  echo "Pruebe '$0 $help' para más información."
}

error_u_user_wrong() # Objetivo 3 Filtra por usuario, mensaje de error que se muestra si llamamos a '-u' con un parámetro que no es un usuario válido del sistema
{
  echo "Warning! se ha invocado al parámetro '-u' sin un nombre de usuario válido. Ejecución abortada"
  echo "Pruebe '$0 $help' para más información."
}

##### Programa principal
while [ "$1" != "" ]; do
  case $1 in
    -h | $help )
      usage
      exit 0
      ;;

    -t )
      shift
      if [ "$1" != "" ]; then
        if [ "$t" == "1" ]; then # Si se ha llamado con anterioridad a '-t', entonces se termina el programa
          error_parameter_duplicated
          exit 1
        fi

        t=1
        aux='^[0-9]+$'
        if [[ $1 =~ $aux ]]; then # Si el valor que acompaña a '-t' es un numero, todo bien
          if [[ $1 > 0 ]]; then # Si el número es mayor que 0, todo bien
            n=$1
          else
            error_option_t_value_no_positive
            exit 1
          fi
        else
          error_option_t_value_no_positive
          exit 1
        fi
      else
        error_option_t_value_no_positive
        exit 1
      fi
      ;;

    -usr ) # Objetivo 2 Solo usuarios con conexion
      if [ "$usr" == "1" ]; then # Si se ha llamado con anterioridad a '-usr', se termina el programa
        error_parameter_duplicated
        exit 1
      fi

      if [ "$u" == "1" ]; then # Si se ha llamado con anterioridad a '-u', se termina el programa porque son incompatibles
        error_usr_u
        exit 1
      fi
      usr=1
      ;;

    -u )
      shift
      if [ "$u" == "1" ]; then # Si se ha llamado con anterioridad a '-u', se termina el programa
        error_parameter_duplicated
        exit 1
      fi

      if [ "$usr" == "1" ]; then # Si se ha llamado con anterioridad a '-usr', se termina el programa porque son incompatibles
        error_usr_u
        exit 1
      fi

      u=1
      aux='^-.*'
      if [ "$1" == "" ]; then # Si a '-u' le acompaña algo, de momento todo bien
        error_u_no_users
        exit 1
      fi

      if [[ $1 =~ $aux ]]; then # Si lo que acompaña a '-u' no es otro parámetro, entonces consideramos que es el nombre de usuario, el primero
        error_u_no_users
        exit 1
      fi

      while [[ ! $1 =~ $aux ]]; do # Mientras hayan usuarios por leer, itera
        if [ "$1" == "" ]; then # Si el nombre de usuario, es vacio, sale del bucle
          break
        fi

        if  grep "^$1:" /etc/passwd > /dev/null 2>&1 ; then # Si el nombre está en el archivo donde se guarda el nombre de los usuarios, entonces existe
          list_defined_by_user="${list_defined_by_user}$1 " # Se añade un usuario más a la lista
          var_u_activated=1 # Decimos al sistema que se ha usado el comando "-u" para que el shift que está al final del while que lee los comandos del programa, no borre el siguiente parametro posicional
          shift # Cambiamos al siguiente valor en la línea de parametros posicionales
        else
          error_u_user_wrong
          exit 1
        fi
      done
      ;;

    -count ) # Objetivo 4 Numero de procesos de usuario
      if [ "$count" == "1" ]; then # Si ya se ha llamado con anterioridad a '-count', se termina el programa
        error_parameter_duplicated
        exit 1
      fi
      count=1
      ;;

    -inv ) # Objetivo 5 Cambios en la ordenacion
      if [ "$inv" == "1" ]; then # Si ya se ha llamado con anterioridad a '-inv', se termina el programa
        error_parameter_duplicated
        exit 1
      fi
      inv=1
      ;;

    -pid ) # Objetivo 5 Cambios en la ordenacion
      if [ "$c" == "1" ]; then # Si ya se ha llamado con anterioridad a '-c', se termina el programa porque son incompatibles
        error_sort
        exit 1
      fi

      if [ "$pid" == "1" ]; then # Si ya se ha llamado con anterioridad a '-pid', se termina el programa
        error_parameter_duplicated
        exit 1
      fi
      pid=1
      ;;

    -c ) # Objetivo 5 Cambios en la ordenacion
      if [ "$pid" == "1" ]; then # Si ya se ha llamado con anterioridad a '-pid', se termina el programa porque son incompatibles
        error_sort
        exit 1
      fi

      if [ "$c" == "1" ]; then # Si ya se ha llamado con anterioridad a '-c', se termina el programa
        error_parameter_duplicated
        exit 1
      fi
      c=1
      ;;

    * ) # Si un valor pasado al programa, no coincide con todos los parámetros descritos anteriormente, entonces se ha escrito mal
      error_parameter_wrong
      exit 1
      ;;
  esac

  if [ "$var_u_activated" == "1" ]; then # Si en esta iteracion se leyó el parámetro '-u' entonces se hace un continue para no borrar el siguiente parametro posicional
    var_u_activated=0    # Lo volvemos a poner a cero para no entrar más aquí
    continue
  fi
  shift
done

users_table # Llamamos a la funcion que nos muestra la tabla 

exit 0 # Salida exitosa del programa