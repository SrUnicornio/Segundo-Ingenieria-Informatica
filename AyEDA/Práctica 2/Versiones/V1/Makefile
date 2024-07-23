# Universidad de La Laguna
# Escuela Superior de Ingeniería y Tecnología
# Grado en Ingeniería Informática
# Asignatura: Algoritmos y Estructuras de Datos Avanzadas
#
# @auth
# @date 03 FEB 2024
# @brief A Makefile example for simple C++ programs
# 

CXX = g++						           
CXXFLAGS = -std=c++23
LDFLAGS =						           

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# Objetive
TARGET = $(BIN_DIR)/JuegoDeLaVida

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
INC_DIRS = $(wildcard $(INC_DIR)/*.hpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Indicate that the all and clean targets do not
# correspond to actual files.
.PHONY: all clean

# The following rule is effectively built into make and
# therefore need not be explicitly specified:
# JuegoDeLaVida_main.o: JuegoDeLaVida_main.cpp

	$(CXX) $(CXXFLAGS) -c $<

# The clean target removes all of the executable files
# and object files produced by the build process
# We can use it for additional housekeeping purposes
clean :
	rm -f *.o JuegoDeLaVida doc
	rm -rf *~ basura b i
	rm -rf a.out
	find . -name '*~' -exec rm {} \;
	find . -name basura -exec rm {} \;