# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.24

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/h3l/Documents/Studium/Master/Sem1/PEAS2/ircom_msp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/h3l/Documents/Studium/Master/Sem1/PEAS2/ircom_msp/build

# Include any dependencies generated for this target.
include library_dev/CMakeFiles/multiply.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include library_dev/CMakeFiles/multiply.dir/compiler_depend.make

# Include the progress variables for this target.
include library_dev/CMakeFiles/multiply.dir/progress.make

# Include the compile flags for this target's objects.
include library_dev/CMakeFiles/multiply.dir/flags.make

library_dev/CMakeFiles/multiply.dir/multiply.cpp.o: library_dev/CMakeFiles/multiply.dir/flags.make
library_dev/CMakeFiles/multiply.dir/multiply.cpp.o: /home/h3l/Documents/Studium/Master/Sem1/PEAS2/ircom_msp/library_dev/multiply.cpp
library_dev/CMakeFiles/multiply.dir/multiply.cpp.o: library_dev/CMakeFiles/multiply.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/h3l/Documents/Studium/Master/Sem1/PEAS2/ircom_msp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object library_dev/CMakeFiles/multiply.dir/multiply.cpp.o"
	cd /home/h3l/Documents/Studium/Master/Sem1/PEAS2/ircom_msp/build/library_dev && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT library_dev/CMakeFiles/multiply.dir/multiply.cpp.o -MF CMakeFiles/multiply.dir/multiply.cpp.o.d -o CMakeFiles/multiply.dir/multiply.cpp.o -c /home/h3l/Documents/Studium/Master/Sem1/PEAS2/ircom_msp/library_dev/multiply.cpp

library_dev/CMakeFiles/multiply.dir/multiply.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/multiply.dir/multiply.cpp.i"
	cd /home/h3l/Documents/Studium/Master/Sem1/PEAS2/ircom_msp/build/library_dev && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/h3l/Documents/Studium/Master/Sem1/PEAS2/ircom_msp/library_dev/multiply.cpp > CMakeFiles/multiply.dir/multiply.cpp.i

library_dev/CMakeFiles/multiply.dir/multiply.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/multiply.dir/multiply.cpp.s"
	cd /home/h3l/Documents/Studium/Master/Sem1/PEAS2/ircom_msp/build/library_dev && /usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/h3l/Documents/Studium/Master/Sem1/PEAS2/ircom_msp/library_dev/multiply.cpp -o CMakeFiles/multiply.dir/multiply.cpp.s

# Object files for target multiply
multiply_OBJECTS = \
"CMakeFiles/multiply.dir/multiply.cpp.o"

# External object files for target multiply
multiply_EXTERNAL_OBJECTS =

library_dev/libmultiply.a: library_dev/CMakeFiles/multiply.dir/multiply.cpp.o
library_dev/libmultiply.a: library_dev/CMakeFiles/multiply.dir/build.make
library_dev/libmultiply.a: library_dev/CMakeFiles/multiply.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/h3l/Documents/Studium/Master/Sem1/PEAS2/ircom_msp/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libmultiply.a"
	cd /home/h3l/Documents/Studium/Master/Sem1/PEAS2/ircom_msp/build/library_dev && $(CMAKE_COMMAND) -P CMakeFiles/multiply.dir/cmake_clean_target.cmake
	cd /home/h3l/Documents/Studium/Master/Sem1/PEAS2/ircom_msp/build/library_dev && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/multiply.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
library_dev/CMakeFiles/multiply.dir/build: library_dev/libmultiply.a
.PHONY : library_dev/CMakeFiles/multiply.dir/build

library_dev/CMakeFiles/multiply.dir/clean:
	cd /home/h3l/Documents/Studium/Master/Sem1/PEAS2/ircom_msp/build/library_dev && $(CMAKE_COMMAND) -P CMakeFiles/multiply.dir/cmake_clean.cmake
.PHONY : library_dev/CMakeFiles/multiply.dir/clean

library_dev/CMakeFiles/multiply.dir/depend:
	cd /home/h3l/Documents/Studium/Master/Sem1/PEAS2/ircom_msp/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/h3l/Documents/Studium/Master/Sem1/PEAS2/ircom_msp /home/h3l/Documents/Studium/Master/Sem1/PEAS2/ircom_msp/library_dev /home/h3l/Documents/Studium/Master/Sem1/PEAS2/ircom_msp/build /home/h3l/Documents/Studium/Master/Sem1/PEAS2/ircom_msp/build/library_dev /home/h3l/Documents/Studium/Master/Sem1/PEAS2/ircom_msp/build/library_dev/CMakeFiles/multiply.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : library_dev/CMakeFiles/multiply.dir/depend

