# Universidad de La Laguna
# Escuela Superior de Ingeniería y Tecnología
# Grado en Ingeniería Informática
# Algoritmos y Estructuras de Datos Avanzadas
#
# @author Alvaro Perez Ramos
# @date 04 Mar 2024 
# @brief A cmake file for Ordenation_Algorithms
#

cmake_minimum_required(VERSION 3.1) 

# set the project name 
project(Ordenation_Algorithms              
    VERSION 1.0
    DESCRIPTION "Ordenation Algorithms"
    LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17) 						
set(CMAKE_CXX_STANDARD_REQUIRED True)

# tell cmake to output binaries here:
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)

# Debug and release flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall-Wpedantic -Weffc++")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# tell cmake where to look for *.h files
include_directories(${PROJECT_SOURCE_DIR}/src)

# Adds an executable target called "Ordenation_Algorithms" to be built from the source files listed in the command invocation.
add_executable(Ordenation_Algorithms
    src/main/main.cc
)

# Add expected compiler features to a target.
target_compile_features(Ordenation_Algorithms PRIVATE cxx_std_17)

    # Add compile options to a target.
target_compile_options(Ordenation_Algorithms PRIVATE -Wall -Wextra -Wpedantic -Weffc++)
