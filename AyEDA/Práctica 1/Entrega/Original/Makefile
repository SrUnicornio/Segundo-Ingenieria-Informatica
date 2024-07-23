# Universidad de La Laguna
# Escuela Superior de Ingeniería y Tecnología
# Grado en Ingeniería Informática
# Asignatura: Algoritmos y Estructuras de Datos Avanzadas
#
# @author Álvaro Pérez Ramos
# @date 03 FEB 2024
# @brief A Makefile example for simple C++ programs
# 

CXX = g++						           
CXXFLAGS = -std=c++23
LDFLAGS =						           

# The all target builds all of the programs handled by the makefile.
all: AutomataCelular

AutomataCelular: *cpp *hpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Indicate that the all and clean targets do not
# correspond to actual files.
.PHONY: all clean

# The following rule is effectively built into make and
# therefore need not be explicitly specified:
# AutomataCelular_main.o: AutomataCelular_main.cpp

	$(CXX) $(CXXFLAGS) -c $<

# The clean target removes all of the executable files
# and object files produced by the build process
# We can use it for additional housekeeping purposes
clean :
	rm -f *.o AutomataCelular doc
	rm -rf *~ basura b i
	rm -rf a.out
	find . -name '*~' -exec rm {} \;
	find . -name basura -exec rm {} \;