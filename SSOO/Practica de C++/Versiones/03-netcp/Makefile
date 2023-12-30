# Universidad de La Laguna
# Escuela Superior de Ingeniería y Tecnología
# Grado en Ingeniería Informática
# SISTEMAS OPERATIVOS
#
# @author Álvaro Pérez Ramos
# @date December 04 2023
# @brief A Makefile example for simple C++ programs
# 

CXX = g++						           
CXXFLAGS = -std=c++23
LDFLAGS =						           

# The all target builds all of the programs handled by the makefile.
all: netcp

netcp: netcp*
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Indicate that the all and clean targets do not
# correspond to actual files.
.PHONY: all clean

# The following rule is effectively built into make and
# therefore need not be explicitly specified:
# netco_main.o: netcp_main.cpp

	$(CXX) $(CXXFLAGS) -c $<

# The clean target removes all of the executable files
# and object files produced by the build process
# We can use it for additional housekeeping purposes
clean :
	rm -f *.o netcp doc
	rm -rf *~ basura b i
	rm -rf a.out
	find . -name '*~' -exec rm {} \;
	find . -name basura -exec rm {} \;