# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/alvaro/Escritorio/PERSONAL/2º carrera/AyEDA/Practicas/Práctica 5/Versiones/V1"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/alvaro/Escritorio/PERSONAL/2º carrera/AyEDA/Practicas/Práctica 5/Versiones/V1/build"

# Include any dependencies generated for this target.
include CMakeFiles/Ordenation_Algorithms.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Ordenation_Algorithms.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Ordenation_Algorithms.dir/flags.make

CMakeFiles/Ordenation_Algorithms.dir/src/main/main.cc.o: CMakeFiles/Ordenation_Algorithms.dir/flags.make
CMakeFiles/Ordenation_Algorithms.dir/src/main/main.cc.o: ../src/main/main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/alvaro/Escritorio/PERSONAL/2º carrera/AyEDA/Practicas/Práctica 5/Versiones/V1/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Ordenation_Algorithms.dir/src/main/main.cc.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Ordenation_Algorithms.dir/src/main/main.cc.o -c "/home/alvaro/Escritorio/PERSONAL/2º carrera/AyEDA/Practicas/Práctica 5/Versiones/V1/src/main/main.cc"

CMakeFiles/Ordenation_Algorithms.dir/src/main/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ordenation_Algorithms.dir/src/main/main.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/alvaro/Escritorio/PERSONAL/2º carrera/AyEDA/Practicas/Práctica 5/Versiones/V1/src/main/main.cc" > CMakeFiles/Ordenation_Algorithms.dir/src/main/main.cc.i

CMakeFiles/Ordenation_Algorithms.dir/src/main/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ordenation_Algorithms.dir/src/main/main.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/alvaro/Escritorio/PERSONAL/2º carrera/AyEDA/Practicas/Práctica 5/Versiones/V1/src/main/main.cc" -o CMakeFiles/Ordenation_Algorithms.dir/src/main/main.cc.s

CMakeFiles/Ordenation_Algorithms.dir/src/main/main.cc.o.requires:

.PHONY : CMakeFiles/Ordenation_Algorithms.dir/src/main/main.cc.o.requires

CMakeFiles/Ordenation_Algorithms.dir/src/main/main.cc.o.provides: CMakeFiles/Ordenation_Algorithms.dir/src/main/main.cc.o.requires
	$(MAKE) -f CMakeFiles/Ordenation_Algorithms.dir/build.make CMakeFiles/Ordenation_Algorithms.dir/src/main/main.cc.o.provides.build
.PHONY : CMakeFiles/Ordenation_Algorithms.dir/src/main/main.cc.o.provides

CMakeFiles/Ordenation_Algorithms.dir/src/main/main.cc.o.provides.build: CMakeFiles/Ordenation_Algorithms.dir/src/main/main.cc.o


# Object files for target Ordenation_Algorithms
Ordenation_Algorithms_OBJECTS = \
"CMakeFiles/Ordenation_Algorithms.dir/src/main/main.cc.o"

# External object files for target Ordenation_Algorithms
Ordenation_Algorithms_EXTERNAL_OBJECTS =

Ordenation_Algorithms: CMakeFiles/Ordenation_Algorithms.dir/src/main/main.cc.o
Ordenation_Algorithms: CMakeFiles/Ordenation_Algorithms.dir/build.make
Ordenation_Algorithms: CMakeFiles/Ordenation_Algorithms.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/alvaro/Escritorio/PERSONAL/2º carrera/AyEDA/Practicas/Práctica 5/Versiones/V1/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Ordenation_Algorithms"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Ordenation_Algorithms.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Ordenation_Algorithms.dir/build: Ordenation_Algorithms

.PHONY : CMakeFiles/Ordenation_Algorithms.dir/build

CMakeFiles/Ordenation_Algorithms.dir/requires: CMakeFiles/Ordenation_Algorithms.dir/src/main/main.cc.o.requires

.PHONY : CMakeFiles/Ordenation_Algorithms.dir/requires

CMakeFiles/Ordenation_Algorithms.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Ordenation_Algorithms.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Ordenation_Algorithms.dir/clean

CMakeFiles/Ordenation_Algorithms.dir/depend:
	cd "/home/alvaro/Escritorio/PERSONAL/2º carrera/AyEDA/Practicas/Práctica 5/Versiones/V1/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/alvaro/Escritorio/PERSONAL/2º carrera/AyEDA/Practicas/Práctica 5/Versiones/V1" "/home/alvaro/Escritorio/PERSONAL/2º carrera/AyEDA/Practicas/Práctica 5/Versiones/V1" "/home/alvaro/Escritorio/PERSONAL/2º carrera/AyEDA/Practicas/Práctica 5/Versiones/V1/build" "/home/alvaro/Escritorio/PERSONAL/2º carrera/AyEDA/Practicas/Práctica 5/Versiones/V1/build" "/home/alvaro/Escritorio/PERSONAL/2º carrera/AyEDA/Practicas/Práctica 5/Versiones/V1/build/CMakeFiles/Ordenation_Algorithms.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/Ordenation_Algorithms.dir/depend

