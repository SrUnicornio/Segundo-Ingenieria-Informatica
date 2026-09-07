cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(p12_AlgortimosDyV)

# Configuración del compilador
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Directorios de los archivos fuente y encabezados
set(SRC_DIR src)
set(INCLUDE_DIR lib)

# Buscar todos los archivos .cc en el directorio src
file(GLOB SOURCES "${SRC_DIR}/*.cpp")

# Definir el ejecutable
add_executable(p12_AlgortimosDyV ${SOURCES})

# Incluir el directorio de headers
target_include_directories(p12_AlgortimosDyV PRIVATE ${INCLUDE_DIR})

# Carpeta de salida para el binario
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)