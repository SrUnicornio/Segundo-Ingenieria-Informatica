#!/bin/bash 

# Parte 4.2 terminado

# Práctica de BASH: Traza de llamadas del sistema en los procesos.

##### Constantes
TITLE="Traza de llamadas del sistema en los procesos. Por Álvaro Pérez Ramos"
SEPARADOR=$'\n=========================================================================\n'

##### Estilos
  TEXT_BOLD=$(tput bold)
  TEXT_GREEN=$(tput setaf 2)
  TEXT_RED=$(tput setaf 1)
  TEXT_BLUE=$(tput setaf 4)
  TEXT_ULINE=$(tput sgr 0 1)
  TEXT_RESET=$(tput sgr0)

##### Variables
  PROGS=""
  PROGRAMS=()
  ARGS=""
  OPCIONES_STO=""
  BOOL_NATTCH=false
  BOOL_PATTCH=false
  BOOL_V=false
  BOOL_VALL=false
  PIDS=()
  RESPUESTA=""

##### Funciones
# Función Error_Exit() para mostrar un mensaje de error y salir del script con un código de error.
Error_Exit() 
{
  # Muestra un mensaje de error y sale del script.
  echo "${SEPARADOR}${TEXT_BOLD}${TEXT_RED}ERROR: ${TEXT_RESET}${TEXT_ULINE}${1}${TEXT_RESET}" 1>&2
  echo "Use $0 -h o --help para más información ${SEPARADOR}"
}

# La función Usage() muestra la ayuda del programa.
Usage ()
{
  # Muestra la ayuda del programa
  cat << _EOF_
  ${SEPARADOR}
  ${TEXT_BOLD}Sintaxis 'scdebug [-h] [-sto arg] [-v | -vall] [-k] [prog [arg ...] ] [-nattch progtoattach ...] [-pattch pid1 ... ' $TEXT_RESET
  
  -h --help               Muestra la ayuda del programa.

  -sto arg                Ejecuta strace con las opciones pasadas como argumento. 
                          Si no se pasa ninguna opción se ejecuta strace con las opciones por defecto 'strace  -o $HOME/.scdebug/$PROGS/trace_UUID.txt PROG (arg)'.

  -v --vall               Muestra la información de la llamada al sistema y el resultado de la llamada al sistema. 
                          Si se pasa -vall se muestran todas las trazas realizadas. 
                          Si se pasa -v se muestra la última traza realizada. 
                          Si no se pasa ningún programa o comando se muestra un mensaje de error.
                          Como se pueden elegir más nombres de comandos el volcado se realizará sobre todos.

  -k                      Tratará de terminar todos los procesos trazadores del usuario, así como todos los procesos trazados.

  PROG args               Programa a ejecutar scdebug, con sus argumentos (si los tiene). 
                          En caso de que no se especifique, se ejecutará el programa scdebug sin argumentos. 
                          Los argumentos de PROG se especifican después de PROG, y se pueden especificar varios argumentos entre comillas simples, 
                          ${TEXT_BOLD}${TEXT_BLUE}Por ejemplo: scdebug PROG 'arg1 arg2 arg3'.${TEXT_RESET}
                          ${TEXT_BOLD}${TEXT_RED}IMPORTANTE: ${TEXT_RESET}${TEXT_BOLD}Los argumentos se pasan entre comillas simples por ejemplo: ${TEXT_ULINE}$0 PROG 'arg1 arg2 arg3'${TEXT_RESET}  
  
  -nattch progtoattach    Monitorizar procesos que ya están en ejecución (modo attach).
                          Se pueda especificar el nombre uno o varios programa/s a monitorizar, y se opte por el proceso del usuario cuya ejecución se inició más recientemente con ese comando.
                          Se le puede pasar con anterioridad el argumento -sto para especificar las opciones de strace.
                          

  -pattch pid1            Puede recibir un número indeterminado de PIDs de procesos a monitorizar con strace.
                          Los nombres de los comandos a utilizar para el registro de las salidas se obtendrán a partir de la información de los procesos dado su pid.

  -rm                     Elimina el directorio .scdebug/PROG con sus archivos. 
                          Si no se pasa ningún programa o comando se muestra eliminará todo el contenido del directorio .scdebug/*
  ${SEPARADOR}
_EOF_
}

# Función que elimina el directorio .scdebug/$PROGS/*
Eliminar_Directorio() 
{
  echo "${SEPARADOR}Se va a eliminar el directorio '$HOME/.scdebug/$PROGS/ con sus archivos' ¿Está seguro? (s/n)"
  read RESPUESTA
  if [[ "$RESPUESTA" == "s" || "$RESPUESTA" == "S" ]]; then
    echo "Se va a eliminar el directorio $HOME/.scdebug/$PROGS/ con sus archivos"
    rm -r $HOME/.scdebug/$PROGS/*
  elif [[ "$RESPUESTA" == "n" || "$RESPUESTA" == "N" ]]; then
    echo "No se va a eliminar el directorio $HOME/.scdebug/$PROGS/"
  else
    Error_Exit "No se ha introducido una respuesta válida"
  fi
}

# Función que obtiene los PID de los procesos en ejecución
Get_PIDs() 
{
  echo -e "${SEPARADOR}${TEXT_BOLD}${TEXT_ULINE}Obteniendo los PID de los procesos en ejecución de $PROGS con los argumentos $ARGS${TEXT_RESET} \n"
  
  PIDS=()
  for pid in $(ps -u $USER -eo pid,cmd | grep -E "\<$PROGS\>|strace" | grep -Ev "grep|-k" | sort -n -k1 | awk '{print $1}'); do
    PIDS+=("$pid")
    echo "PID: $pid, CMD: $(ps -p $pid -o cmd=)"
  done

  if [ ${#PIDS[@]} -eq 0 ]; then
    echo "$SEPARADOR"
    Error_Exit "No se encontraron procesos con el nombre $PROGS en ejecución."
    exit 1
  fi
  
  echo -e "\n${TEXT_BOLD}${TEXT_ULINE}Se han obtenido los PID de los procesos en ejecución${TEXT_RESET}${SEPARADOR}"
}

# Función que lanza strace
Launch_Strace() 
{
  if [[ "$BOOL_PATTCH" == true || "$BOOL_NATTCH" == true ]]; then
    echo "strace $1 -p $2 -o $3"
    strace $1 -p $2 -o "$3" 2>"$3.salida" || (Error_Exit "No se ha podido ejecutar el attach con strace $1 -p $2 -o $3"; rm "$3")
  elif [[ "$BOOL_NATTCH" == false ]]; then
    if [ -x "$3" ]; then
      echo "strace $1 -o $2 ./$3 $4"
      strace $1 -o "$2" ./$3 $4|| Error_Exit "No se ha podido ejecutar strace $1 -o $2 ./$3 $4"
    elif [ $(command -v "$3") ]; then
      echo "strace $1 -o $2 $3 $4"
      strace $1 -o "$2" $3 $4 || Error_Exit "No se ha podido ejecutar strace $1 -o $2 $3 $4"
    else
      Error_Exit "El programa $3 no existe o no es ejecutable"
      exit 1
    fi
  fi
}


# Fuinción STO() para ejecutar strace con las opciones $OPCIONES_STO
STO()
{
  if [[ -x "$PROGS" || $(command -v "$PROGS") ]]; then
    mkdir -p "$HOME/.scdebug/$PROGS"
    TRACE_FILE="$HOME/.scdebug/$PROGS/trace_$(uuidgen).txt"
    echo "Se va a monitorizar el proceso '$PROGS $ARGS' y las opciones de STO '$OPCIONES_STO'"
    Launch_Strace "$OPCIONES_STO" "$TRACE_FILE" "$PROGS" "$ARGS" &
    echo "${SEPARADOR}El PID del proceso en segundo plano es $!"
  else
    Error_Exit "El programa $PROGS no existe o no es ejecutable"
    exit 1
  fi
}

# Función NATTCH() para monitorizar procesos que ya están en ejecución (modo attach)
NATTCH()
{
  echo "Se ha pasado la opción -nattch"
  if [[ -x "$PROGS" || $(command -v "$PROGS") ]]; then
    mkdir -p "$HOME/.scdebug/$PROGS"
    Get_PIDs
    if [ ${#PIDS[@]} -gt 0 ]; then
      for PID in "${PIDS[@]}"; do
        TRACE_FILE="$HOME/.scdebug/$PROGS/trace_$(uuidgen).txt"
        echo -e "\nSe va a monitorizar el proceso '$PROGS ${ARGS}' con PID $PID y las opciones de STO'$OPCIONES_STO'"
        Launch_Strace "$OPCIONES_STO" "$PID" "$TRACE_FILE" &
        sleep 0.5
      done
    else
      Error_Exit "No se encontraron procesos con el nombre $PROGS en ejecución pues PIDS esta vacio."
      exit 1
    fi
  else
    Error_Exit "El programa $PROGS no existe o no es ejecutable"
    exit 1
  fi
}

# Función PATTCH() para monitorizar procesos que ya están en ejecución (modo attach)
PATTCH()
{
  echo "Se ha pasado la opción -pattch"
  if [ ${#PIDS[@]} -gt 0 ]; then
    for PID in "${PIDS[@]}"; do
      mkdir -p "$HOME/.scdebug/$PID"
      TRACE_FILE="$HOME/.scdebug/$PID/trace_$(uuidgen).txt"
      echo "${SEPARADOR}Se va a monitorizar el proceso asociado con el PID $PID CMD: $(ps -p $PID -o cmd=) y las opciones de STO '$OPCIONES_STO'"
      Launch_Strace "$OPCIONES_STO" "$PID" "$TRACE_FILE" &
      sleep 0.5
    done
  else
    Error_Exit "No se encontraron procesos con el nombre $PROGS en ejecución."
    exit 1
  fi
}

# Función Scdebug() verifica si se ha pasado la opción -sto, -nattch o -pattch
Scdebug() 
{
  if [[ "$BOOL_PATTCH" == true ]]; then
    PATTCH
  elif [[ "$BOOL_NATTCH" == true ]]; then
    NATTCH
  else
    STO
  fi
}

# Función Trazas() para consultar las trazas ya realizadas de un programa por -v o -vall
Trazas()
{
  # Comprobamos si se ha pasado la opción -v o -vall
  if [[ "$BOOL_V" == true ]]; then
    echo "${SEPARADOR}Se ha pasado la opción -v y el programa $PROGS"
    echo -e "\nSe va a mostrar la última traza realizada del programa $PROGS en el directorio $HOME/.scdebug/$PROGS \n"

    # Comprobamos si existe el directorio $HOME/.scdebug/$PROGS
    if [ -d $HOME/.scdebug/$PROGS ]; then
        # Obtenemos el archivo con fecha de modificación más reciente
        ARCHIVO=$(ls -t $HOME/.scdebug/$PROGS | head -n1)

        # Mostramos el contenido del archivo
        echo "============== COMMAND: $PROGS ======================="
        echo "============== TRACE FILE: $ARCHIVO ================="
        echo "============== TIME: $(stat -c %y $HOME/.scdebug/$PROGS/$ARCHIVO) =============="
    
    else
        Error_Exit "No existe el directorio .scdebug/$PROGS"
    fi
  elif [[ "$BOOL_VALL" == true ]]; then
    echo "$SEPARADOR"
    echo -e "Se ha pasado la opción -vall y el programa $PROGS \n"
    echo "Se van a mostrar todas las trazas realizadas del programa $PROGS en el directorio $HOME/.scdebug/$PROGS \n"

    # Comprobamos si existe el directorio $HOME/.scdebug/$PROGS
    if [ -d $HOME/.scdebug/$PROGS ]; then
      # Obtenemos los archivos del directorio $HOME/.scdebug/$PROGS ordenados de más reciente a más antiguo
      ARCHIVOS=($(ls -t $HOME/.scdebug/$PROGS))

      # Mostramos el contenido de cada archivo
      for ARCHIVO in "${ARCHIVOS[@]}"; do
        echo "============== COMMAND: $PROGS ======================="
        echo "============== TRACE FILE: $ARCHIVO ================="
        echo "============== TIME: $(stat -c %y $HOME/.scdebug/$PROGS/$ARCHIVO) =============="
      done
    else
      Error_Exit "No existe el directorio .scdebug/$PROGS"
    fi
  fi
}

# Función Matar_Porcesos() para terminar todos los procesos trazadores del usuario, así como todos los procesos trazados.
Matar_Porcesos()
{
  Get_PIDs
  if [ ${#PIDS[@]} -gt 0 ]; then
    for PID in "${PIDS[@]}"; do 
      echo "$SEPARADOR"
      echo "Se va a terminar el proceso '$PROGS ${ARGS}' con PID $PID"
      # Terminamos el proceso
      echo "kill -TERM $PID"
      kill -TERM $PID 2>/dev/null
    done
    sleep 4
    for PID in "${PIDS[@]}"; do 
      echo "$SEPARADOR"
      echo "Se va a terminar el proceso '$PROGS ${ARGS}' con PID $PID"
      # Terminamos el proceso
      echo "kill -KILL $PID"
      kill -KILL $PID 2>/dev/null
    done 
  else
    echo "$SEPARADOR"
    Error_Exit "No se encontraron procesos con el nombre $PROGS en ejecución."
  fi
}

# Finución Show_Process_Info() para mostrar los procesos del usuario
Show_Process_Info() 
{
  # Muestra los procesos del usuario
  echo "${SEPARADOR}Se van a mostrar los procesos del usuario $USER, ordenados por tiempo de inicio:"
  # Obtenemos los PIDs de los procesos del usuario
  Get_PIDs
  echo "$SEPARADOR"
  # Mostramos los procesos del usuario, ordenados por tiempo de inicio
  for PID in "${PIDS[@]}"; do
    # Obtenemos el nombre del proceso bajo trazado (tracee)
    TRACEE=$(ps -p $PID -o cmd= | tail -n 1)
    if [ -z "$TRACEE" ]; then
      TRACEE="N/A"
    fi

    # Obtenemos el PID del proceso trazador (tracer)
    TRACER_PID=$(cat /proc/$PID/status | grep TracerPid | grep -v grep | awk '{print $2}') 2>/dev/null
    if [ -z "$TRACER_PID" ]; then
      TRACER_PID=0
    fi

    # Obtenemos el nombre del proceso trazador (tracer)
    if [ "$TRACER_PID" -eq 0 ]; then
      CMD_TRACER="N/A"
    else
      CMD_TRACER=$(ps -p $TRACER_PID -o cmd= | tail -n 1)
      if [ -z "$CMD_TRACER" ]; then
        CMD_TRACER="N/A"
      fi
    fi

    # Mostramos el PID, nombre del proceso bajo trazado (tracee), PID y nombre del proceso trazador (tracer)
    echo "${TEXT_BOLD}${TEXT_BLUE}${TEXT_ULINE}PID:${TEXT_RESET} $PID ${TEXT_BOLD}${TEXT_BLUE}${TEXT_ULINE}TRACEE:${TEXT_RESET} $TRACEE ${TEXT_BOLD}${TEXT_BLUE}${TEXT_ULINE}TRACER_PID:${TEXT_RESET} $TRACER_PID ${TEXT_BOLD}${TEXT_BLUE}${TEXT_ULINE}CMD_TRACER:${TEXT_RESET} $CMD_TRACER"
  done | sort -k4,4n
  echo "$SEPARADOR"
}

##### Programa principal
main()
{
  echo "${SEPARADOR}${TITLE}${SEPARADOR}"

  # Comprobamos que se haya pasado algún argumento
  if [ $# -eq 0 ]; then
    Error_Exit "Faltan argumentos"
  fi

  while [ "$1" != "" ]; do
    case "$1" in
      -rm )
        shift
        if [[ -x "$1" || $(command -v "$1") ]]; then
          PROGS="$1"
          echo "Se ha pasado el programa o comando a eliminar: $PROGS"
          Eliminar_Directorio
          exit 0
        else
          echo "$SEPARADOR"
          Error_Exit "Falta el programa o comando a eliminar."
          exit 1
        fi
        ;;
      -h | --help)
        Usage
        exit 0
        ;;
      -sto)
        shift
        if [ -n "$1" ]; then
          OPCIONES_STO="$1"
          shift
        else
          Error_Exit "No se ha pasado ningún argumento para la opción -sto"
        fi
        ;;
      -v | -vall)
        if [[ "$1" == "-v" ]]; then
          echo "Se ha pasado la opción -v"
          BOOL_V=true
        elif [[ "$1" == "-vall" ]]; then
          echo "Se ha pasado la opción -vall"
          BOOL_VALL=true
        fi
        shift
        while [[ "$1" != "" ]]; do
          PROGRAMS+=("$1")
          shift
        done
        for ((i=0; i<${#PROGRAMS[@]}; i++)); do
          PROGS="${PROGRAMS[$i]}"
          if [[ ! -x "$PROGS" || ! $(command -v "$PROGS") ]]; then
            Error_Exit "$PROGS no es un programa válido"
            continue
          fi
          Trazas
        done
        exit 0
        ;;
      -k)
        shift
        if [[ "$1" == "-"* || -z "$1" ]]; then
          Error_Exit "$1 no es un programa válido"
          exit 1
        fi
        PROGS="$1"
        Matar_Porcesos
        exit 0
        ;;
      -nattch)
        BOOL_NATTCH=true
        shift
        if [[ "$1" == "-"* || -z "$1" ]]; then
          Error_Exit "$1 no es un programa válido para -nattch"
          exit 1
        fi
        ;;
      -pattch)
        echo "Se ha pasado la opción -pattch"
        BOOL_PATTCH=true
        shift
        while [ "$1" != "" ]; do
          if [[ "$1" =~ ^[0-9]+$ ]]; then # "$1" =~ ^[0-9]+$ hace que se compruebe si $1 es un número
            PIDS+=("$1")
            shift
          else
            echo "$SEPARADOR"
            Error_Exit "El valor $1 no es un PID válido"
            shift
            continue
          fi
        done
        Scdebug
        exit 0
        ;;
      *)
        while [[ "$1" != "" ]]; do
          PROGRAMS+=("$1")
          shift
        done
    esac
  done

  # Comprobamos si se ha pasado algún programa o comando
  for ((i=0; i<${#PROGRAMS[@]}; i++)); do
    PROGS="${PROGRAMS[$i]}"
    
    # Comprobamos si hay argumentos
    if [ "$i" -lt $((${#PROGRAMS[@]} - 1)) ]; then
      ARGS="${PROGRAMS[$((i+1))]}"
      i=$((i+1)) # Avanzamos al siguiente elemento
    else
      ARGS=""
    fi

    Scdebug
    sleep 0.5
    Show_Process_Info
done
  sleep 0.5
  exit 0
}

main "$@"
