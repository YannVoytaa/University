# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/yannvoytaa/CLionProjects/lab8

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yannvoytaa/CLionProjects/lab8

# Include any dependencies generated for this target.
include CMakeFiles/parent-fifo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/parent-fifo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/parent-fifo.dir/flags.make

CMakeFiles/parent-fifo.dir/parent-fifo.c.o: CMakeFiles/parent-fifo.dir/flags.make
CMakeFiles/parent-fifo.dir/parent-fifo.c.o: parent-fifo.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/yannvoytaa/CLionProjects/lab8/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/parent-fifo.dir/parent-fifo.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/parent-fifo.dir/parent-fifo.c.o   -c /home/yannvoytaa/CLionProjects/lab8/parent-fifo.c

CMakeFiles/parent-fifo.dir/parent-fifo.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/parent-fifo.dir/parent-fifo.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/yannvoytaa/CLionProjects/lab8/parent-fifo.c > CMakeFiles/parent-fifo.dir/parent-fifo.c.i

CMakeFiles/parent-fifo.dir/parent-fifo.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/parent-fifo.dir/parent-fifo.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/yannvoytaa/CLionProjects/lab8/parent-fifo.c -o CMakeFiles/parent-fifo.dir/parent-fifo.c.s

# Object files for target parent-fifo
parent__fifo_OBJECTS = \
"CMakeFiles/parent-fifo.dir/parent-fifo.c.o"

# External object files for target parent-fifo
parent__fifo_EXTERNAL_OBJECTS =

parent-fifo: CMakeFiles/parent-fifo.dir/parent-fifo.c.o
parent-fifo: CMakeFiles/parent-fifo.dir/build.make
parent-fifo: liberr.a
parent-fifo: CMakeFiles/parent-fifo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/yannvoytaa/CLionProjects/lab8/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable parent-fifo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/parent-fifo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/parent-fifo.dir/build: parent-fifo

.PHONY : CMakeFiles/parent-fifo.dir/build

CMakeFiles/parent-fifo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/parent-fifo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/parent-fifo.dir/clean

CMakeFiles/parent-fifo.dir/depend:
	cd /home/yannvoytaa/CLionProjects/lab8 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yannvoytaa/CLionProjects/lab8 /home/yannvoytaa/CLionProjects/lab8 /home/yannvoytaa/CLionProjects/lab8 /home/yannvoytaa/CLionProjects/lab8 /home/yannvoytaa/CLionProjects/lab8/CMakeFiles/parent-fifo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/parent-fifo.dir/depend

