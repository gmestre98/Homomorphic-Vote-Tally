# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/gmestre/Desktop/Homomorphic-Vote-Tally/Admin

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gmestre/Desktop/Homomorphic-Vote-Tally/Admin

# Include any dependencies generated for this target.
include CMakeFiles/Generations.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Generations.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Generations.dir/flags.make

CMakeFiles/Generations.dir/generations.cpp.o: CMakeFiles/Generations.dir/flags.make
CMakeFiles/Generations.dir/generations.cpp.o: generations.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/gmestre/Desktop/Homomorphic-Vote-Tally/Admin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Generations.dir/generations.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Generations.dir/generations.cpp.o -c /home/gmestre/Desktop/Homomorphic-Vote-Tally/Admin/generations.cpp

CMakeFiles/Generations.dir/generations.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Generations.dir/generations.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/gmestre/Desktop/Homomorphic-Vote-Tally/Admin/generations.cpp > CMakeFiles/Generations.dir/generations.cpp.i

CMakeFiles/Generations.dir/generations.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Generations.dir/generations.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/gmestre/Desktop/Homomorphic-Vote-Tally/Admin/generations.cpp -o CMakeFiles/Generations.dir/generations.cpp.s

# Object files for target Generations
Generations_OBJECTS = \
"CMakeFiles/Generations.dir/generations.cpp.o"

# External object files for target Generations
Generations_EXTERNAL_OBJECTS =

libGenerations.a: CMakeFiles/Generations.dir/generations.cpp.o
libGenerations.a: CMakeFiles/Generations.dir/build.make
libGenerations.a: CMakeFiles/Generations.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/gmestre/Desktop/Homomorphic-Vote-Tally/Admin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libGenerations.a"
	$(CMAKE_COMMAND) -P CMakeFiles/Generations.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Generations.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Generations.dir/build: libGenerations.a

.PHONY : CMakeFiles/Generations.dir/build

CMakeFiles/Generations.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Generations.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Generations.dir/clean

CMakeFiles/Generations.dir/depend:
	cd /home/gmestre/Desktop/Homomorphic-Vote-Tally/Admin && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/gmestre/Desktop/Homomorphic-Vote-Tally/Admin /home/gmestre/Desktop/Homomorphic-Vote-Tally/Admin /home/gmestre/Desktop/Homomorphic-Vote-Tally/Admin /home/gmestre/Desktop/Homomorphic-Vote-Tally/Admin /home/gmestre/Desktop/Homomorphic-Vote-Tally/Admin/CMakeFiles/Generations.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Generations.dir/depend
