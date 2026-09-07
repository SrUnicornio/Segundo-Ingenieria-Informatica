cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(p06_automata_simulator)

# Configuración del compilador
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Directorios de los archivos fuente y encabezados
set(SRC_DIR src)
set(INCLUDE_DIR includes)

# Buscar todos los archivos .cc en el directorio src
file(GLOB SOURCES "${SRC_DIR}/*.cc")

# Definir el ejecutable
add_executable(p06_automata_simulator ${SOURCES})

# Incluir el directorio de headers
target_include_directories(p06_automata_simulator PRIVATE ${INCLUDE_DIR})

# Carpeta de salida para el binario
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)