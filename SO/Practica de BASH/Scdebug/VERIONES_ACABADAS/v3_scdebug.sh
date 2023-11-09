#!./ptbash

# parte 4.3 

# Práctica de BASH: Traza de llamadas del sistema en los procesos.
# Álvaro Pérez Ramos
# 13/10/2023 - 07/11/2023

##### Constantes
TITLE="Traza de llamadas del sistema en los procesos. Por Álvaro Pérez Ramos"
SEPARADOR=$'\n==================================================================================================================================================\n'

##### Estilos
  TEXT_BOLD=$(tput bold)
  TEXT_GREEN=$(tput setaf 2)
  TEXT_RED=$(tput setaf 1)
  TEXT_BLUE=$(tput setaf 4)
  TEXT_ULINE=$(tput sgr 0 1)
  TEXT_CURSIVA=$(tput sitm)
  TEXT_RESET=$(tput sgr0)

##### Variables
  PROGRAMS=()
  PROGS=""
  ARGS=""

  OPCIONES_STO=""
  BOOL_NATTCH=false
  BOOL_PATTCH=false
  BOOL_V=false
  BOOL_VALL=false
  PIDS=()

  RESPUESTA=""
  commName=""
  OPT_SM=""
  BOOL_INV=false

##### Funciones
# Función Error_Exit() para mostrar un mensaje de error y salir del script con un código de error.
Error_Exit() 
{
  # Muestra un mensaje de error y sale del script.
  echo "${SEPARADOR}${TEXT_BOLD}${TEXT_RED}ERROR: ${TEXT_RESET}${TEXT_ULINE}${1}${TEXT_RESET}" 1>&2
  echo "Use $0 -h o --help para más información ${SEPARADOR}"
}

# Función Usage() muestra la ayuda del programa.
Usage ()
{
  # Muestra la ayuda del programa
  echo "
  ${SEPARADOR}
  ${TEXT_BOLD}Sintaxis 'scdebug [-h] [-sto arg] [-v | -vall] [-k] [prog [arg ...] ] [-nattch progtoattach ...] [-pattch PID1 ... ' $TEXT_RESET
  
  -h --help               Muestra la ayuda del programa.

  -sto arg                Ejecuta strace con las opciones pasadas como argumento. ${TEXT_BOLD}${TEXT_BLUE}'strace OPCIONES_STO -o $HOME/.scdebug/PROG/trace_UUIDGEN.txt PROG ARGS'${TEXT_RESET}
                          Si no se pasa ninguna opción se ejecuta strace con las opciones por defecto ${TEXT_BOLD}${TEXT_BLUE}'strace -o $HOME/.scdebug/PROG/trace_UUIDGEN.txt PROG ARGS'${TEXT_RESET}.

  -v --vall               Muestra la información de la llamada al sistema y el resultado de la llamada al sistema. 
                          Si se pasa -vall se muestran todas las trazas realizadas. Desde la última hasta la primera
                          Si se pasa -v se muestra la última traza realizada. 
                          Como se pueden elegir más de un programa y/o comando el volcado se realizará sobre todos.

  -k                      Tratará de terminar todos los procesos trazadores del usuario, así como todos los procesos trazados.

  PROG [args...]          Programa/s a ejecutar con scdebug, con sus argumentos (si los tiene). 
                          Los argumentos de PROG se especifican después de PROG.
                          ${TEXT_BOLD}${TEXT_RED}IMPORTANTE: ${TEXT_RESET}${TEXT_BOLD}Los argumentos se pasan entre comillas simples por ejemplo: ${TEXT_ULINE}$0 PROG 'arg1 arg2 arg3'${TEXT_RESET}
                          Si realiza ${TEXT_BOLD}${TEXT_CURSIVA}$0 PROG 'arg1 arg2' PROG2 'arg1'${TEXT_RESET} ${TEXT_ULINE}se realizaria los trazados primero de PROG 'arg1 arg2' y luego de PROG2 'arg1'${TEXT_RESET}
  
  -nattch progtoattach    Monitorizar procesos que ya están en ejecución (modo attach).
                          Se pueda especificar el nombre uno o varios programa/s a monitorizar, y se opte por el proceso del usuario cuya ejecución se inició más recientemente con ese comando.
                          Se le puede pasar con anterioridad el argumento -sto para especificar las opciones de strace.
                          

  -pattch PID1            Puede recibir un número indeterminado de PIDs de procesos a monitorizar con strace.
                          Los nombres de los comandos a utilizar para el registro de las salidas se obtendrán a partir de la información de los procesos dado su PID.

  -rm                     Elimina el directorio .scdebug/PROG con sus archivos. 
                          Si no se pasa ningún programa o comando se muestra eliminará todo el contenido del directorio .scdebug/*
  ${SEPARADOR}
  ${TEXT_BOLD}Sintaxis para activar opciones excluyentes salvo para -h y -k ${TEXT_BLUE}'scdegug [-h] [-k] -S commName prog [arg...]' 'scdebug [-h] [-k] -g | -gc | -ge [-inv]'${TEXT_RESET} 
  -S commName             La opción -S va acompañada obligatoriamente de un nombre de comando commName, que usaremos para forzar el nombre de comando de nuestro proceso monitorizado. 
                          Además es obligatorio especificar una programa a ejecutar con sus posibles argumentos: prog [arg ...]

  -g | -gc | -ge [-inv]   El trabajo a realizar se hará sobre la lista de PIDs de los procesos del usuario que estén detenidos y cuyo nombre de comando comience por “traced_”, es decir, la lista de PIDs del grupo de procesos preparados por acciones STOP previas
  
  Si se utiliza la opción -g, la monitorización se hace de forma normal ${TEXT_BOLD}${TEXT_BLUE}'strace -o $HOME/.scdebug/PROG/trace_UUIDGEN.txt PROG ARGS'${TEXT_RESET}.
  
  Si se usan las opciones -gc o -ge, ${TEXT_BOLD}${TEXT_BLUE}'strace -c -U'${TEXT_RESET} para que la salida sea una tabla de datos con información sobre las llamadas del sistema realizadas. 
  Junto con la opción -c utilizaremos la opción -U para obtener los parámetros:
  Nombre de la llamada del sistema (name), tiempo máximo en la llamada (max-time), el tiempo total (total-time), número de llamadas (calls) y número de errores (errs). 
  Strace no usará la opción -o para guardar a un archivo, sino que haremos una redirección de la salida error.  
  ${SEPARADOR}"
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
  elif [[ "$BOOL_PATTCH" == false && "$BOOL_NATTCH" == false ]]; then
    if [ -x "$3" ]; then
      echo "strace $1 -o $2 ./$3 $4"
      strace $1 -o "$2" ./$3 $4 2>&1 || Error_Exit "No se ha podido ejecutar strace $1 -o $2 ./$3 $4"
    elif [ $(which "$3") ]; then
      echo "strace $1 -o $2 $3 $4"
      strace $1 -o "$2" $3 $4 2>&1 || Error_Exit "No se ha podido ejecutar strace $1 -o $2 $3 $4"
    else
      Error_Exit "El programa $3 no existe o no es ejecutable"
      exit 1
    fi
  fi
}

# Fuinción STO() para ejecutar strace con las opciones $OPCIONES_STO
STO()
{
  if [[ -x "$PROGS" || $(which "$PROGS") ]]; then
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
  if [[ -x "$PROGS" || $(which "$PROGS") ]]; then
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
        echo -e "\n============== COMMAND: $PROGS ======================="
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

# Funución Show_Process_Info() para mostrar los procesos del usuario
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
    TRACER_PID=$(cat /proc/$PID/status | grep TracerPID | grep -v grep | awk '{print $2}') 2>/dev/null
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

# Función Action_Stop() para detener temporalmente el script y ejecutar el programa a monitorizar
Action_Stop() 
{
  echo "Se ha pasado el programa o comando $PROGS con ARGS=$ARGS para Action_Stop() con el nombre de comando $commName"
  
  # Comprobamos si se ha pasado el nombre del comando y al menos un argumento
  if [[ -z "$commName" || -z "$PROGS" ]]; then
    Error_Exit "Falta el nombre del comando o el programa a ejecutar."
    exit 1
  fi

  # Forzamos el nombre de comando
  echo -n "traced_$commName" > /proc/$$/comm

  # Detenemos temporalmente el script
  kill -SIGSTOP $$

  # Reanudamos la ejecución con el programa a monitorizar
  if [ -x "$PROGS" ]; then
    echo "exec ./$PROGS $ARGS"
    exec ./$PROGS $ARGS
  elif [ $(which "$PROGS") ]; then
    echo "exec $PROGS $ARGS"
    exec $PROGS $ARGS
  else
    Error_Exit "El programa $PROGS no existe o no es ejecutable"
    exit 1
  fi
}

# Función Start_Monitor() para iniciar la monitorización del grupo de procesos detenidos
Start_Monitor() 
{
  case "$OPT_SM" in
    -g)
      # Monitorización de procesos detenidos con strace
      for PID in $(pgrep -u $USER '^traced_'); do
        mkdir -p "$HOME/.scdebug/$PID"
        # Llamada a strace en segundo plano y esperar a que termine
        strace -p $PID -o "$HOME/.scdebug/$PID/trace_$(uuidgen).txt" &
        kill -SIGCONT $PID
        wait $!
      done
      ;;
    -gc | -ge)
      # Monitorización de procesos detenidos con opciones -gc o -ge
      local summary=()
      for PID in $(pgrep -u $USER '^traced_'); do
        summary=()
        echo "PID: $PID CMD: $(ps -p $PID -o cmd=)"
        # Llamada a strace en segundo plano con opciones -c -U y esperar a que termine
        strace -c -U name,max-time,total-time,calls,errors -p $PID 2>strace_summary.txt &
        kill -SIGCONT $PID
        wait $!
        while read -r line; do
          if [[ "$line" == "---"* ]]; then
            summary+=("")
            continue
          fi
          # Extraer el nombre de la llamada del sistema con el mayor max-time
          name=$(echo "$line" | awk '{print $1}')
          max_time=$(echo "$line" | awk '{print $2}')
          total_time=$(echo "$line" | awk '{print $3}')
          calls=$(echo "$line" | awk '{print $4}')
          errs=$(echo "$line" | awk '{print $5}')
          summary+=("\nPID: $PID \tName: $name \tMax_Time: $max_time \tTotal_Time: $total_time \tCalls: $calls \tErrors: $errs")
        done < <(tail -n +2 strace_summary.txt)

        # Si se ha pasado la opción -gc, se ordena por la columna Calls: de mayor a menor
        if [[ "$OPT_SM" == "-gc" ]]; then
          if [[ "$BOOL_INV" == true ]]; then
            echo -e "${summary[@]}" | sort -k10,10nr
          else
            echo -e "${summary[@]}" | sort -k10,10n
          fi

        # Si se ha pasado la opción -ge, se ordena por la columna Errors: de mayor a menor
        elif [[ "$OPT_SM" == "-ge" ]]; then
          if [[ "$BOOL_INV" == true ]]; then
            echo -e "${summary[@]}" | sort -k12,12nr
          else
            echo -e "${summary[@]}" | sort -k12,12n
          fi
        fi


        sleep 0.5
        rm strace_summary.txt
      done

      ;;
    *)
      # Opción no válida
      echo "Opción no válida: $OPT_SM"
      ;;
  esac
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
        if [[ -x "$1" || $(which "$1") ]]; then
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
        shift
        case "$1" in
          -S )
            shift
            if [[ "$1" == "-"* || -z "$1" ]]; then
              Error_Exit "$1 no es un nombre de comando válido para -S"
              exit 1
            fi
            commName="$1"
            shift
            if [[ "$1" == "-"* || -z "$1" ]]; then
              Error_Exit "$1 no es un programa válido para -S"
              exit 1
            fi
            PROGS="$1"
            ARGS="$2"
            Action_Stop
            ;;
          -g | -gc | -ge )
            OPT_SM="$1"
            shift
            if [[ "$1" == "-inv" ]]; then
              BOOL_INV=true
              shift
            fi
            Start_Monitor
            ;;
          *)
            exit 0
            ;;
        esac
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
        for i in "${!PROGRAMS[@]}"; do
          PROGS="${PROGRAMS[$i]}"
          if [[ -x "$PROGS" || $(which "$PROGS") ]]; then
            Trazas
          else
            Error_Exit "$PROGS no es un programa válido"
            continue
          fi
        done
        exit 0
        ;;
      -k)
        shift
        case "$1" in
          -S )
            shift
            if [[ "$1" == "-"* || -z "$1" ]]; then
              Error_Exit "$1 no es un nombre de comando válido para -S"
              exit 1
            fi
            commName="$1"
            shift
            if [[ "$1" == "-"* || -z "$1" ]]; then
              Error_Exit "$1 no es un programa válido para -S"
              exit 1
            fi
            PROGS="$1"
            ARGS="$2"
            Action_Stop
            ;;
          -g | -gc | -ge )
            OPT_SM="$1"
            shift
            if [[ "$1" == "-inv" ]]; then
              BOOL_INV=true
              shift
            fi
            Start_Monitor
            ;;
          *)
            PROGS="$1"
            ;;
        esac
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
      -S )
        shift
        if [[ "$1" == "-"* || -z "$1" ]]; then
          Error_Exit "$1 no es un nombre de comando válido para -S"
          exit 1
        fi
        commName="$1"
        shift
        if [[ "$1" == "-"* || -z "$1" ]]; then
          Error_Exit "$1 no es un programa válido para -S"
          exit 1
        fi
        PROGS="$1"
        ARGS="$2"
        Action_Stop
        ;;
      -g | -gc | -ge )
        OPT_SM="$1"
        shift
        if [[ "$1" == "-inv" ]]; then
          BOOL_INV=true
          shift
        fi
        Start_Monitor
        ;;
      *)
        while [[ "$1" != "" ]]; do
          PROGRAMS+=("$1")
          shift
        done
    esac
  done

  # Comprobamos si se ha pasado algún programa o comando
  for i in "${!PROGRAMS[@]}"; do
    PROGS="${PROGRAMS[$i]}"
    
    if [ -x "$PROGS" ]; then
      if [[ "${PROGRAMS[$i+1]}" == "-"* ]]; then
        ARGS="${PROGRAMS[$i+1]}"
        ((i++))
      else
        ARGS=""
      fi
    elif [ $(which "$PROGS") > /dev/null 2>&1 ]; then
      if [ "$i" -lt $((${#PROGRAMS[@]} - 1)) ]; then
        ARGS="${PROGRAMS[$((i+1))]}"
        ((i++)) # Avanzamos al siguiente elemento
      else
        ARGS=""
      fi
    else
      Error_Exit "$PROGS no es un programa válido"
      continue
    fi

    # Llama a las funciones Scdebug y Show_Process_Info aquí
    Scdebug
    sleep 0.5
    Show_Process_Info

    PROGS=""
    ARGS=""
  done


  sleep 0.5
  exit 0
}

main "$@"