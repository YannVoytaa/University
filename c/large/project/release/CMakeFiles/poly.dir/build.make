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


# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_SOURCE_DIR = /home/yannvoytaa/duze/IPP-jw429685

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yannvoytaa/duze/IPP-jw429685/release

# Include any dependencies generated for this target.
include CMakeFiles/poly.dir/depend.make

# Include the compile flags for this target's objects.
include CMakeFiles/poly.dir/flags.make

CMakeFiles/poly.dir/src/poly.c.o: CMakeFiles/poly.dir/flags.make
CMakeFiles/poly.dir/src/poly.c.o: ../src/poly.c
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/poly.dir/src/poly.c.o   -c /home/yannvoytaa/duze/IPP-jw429685/src/poly.c

CMakeFiles/poly.dir/src/poly.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/poly.dir/src/poly.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/yannvoytaa/duze/IPP-jw429685/src/poly.c > CMakeFiles/poly.dir/src/poly.c.i

CMakeFiles/poly.dir/src/poly.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/poly.dir/src/poly.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/yannvoytaa/duze/IPP-jw429685/src/poly.c -o CMakeFiles/poly.dir/src/poly.c.s

CMakeFiles/poly.dir/src/calc.c.o: CMakeFiles/poly.dir/flags.make
CMakeFiles/poly.dir/src/calc.c.o: ../src/calc.c
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/poly.dir/src/calc.c.o   -c /home/yannvoytaa/duze/IPP-jw429685/src/calc.c

CMakeFiles/poly.dir/src/calc.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/poly.dir/src/calc.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/yannvoytaa/duze/IPP-jw429685/src/calc.c > CMakeFiles/poly.dir/src/calc.c.i

CMakeFiles/poly.dir/src/calc.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/poly.dir/src/calc.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/yannvoytaa/duze/IPP-jw429685/src/calc.c -o CMakeFiles/poly.dir/src/calc.c.s

CMakeFiles/poly.dir/src/Vector.c.o: CMakeFiles/poly.dir/flags.make
CMakeFiles/poly.dir/src/Vector.c.o: ../src/Vector.c
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/poly.dir/src/Vector.c.o   -c /home/yannvoytaa/duze/IPP-jw429685/src/Vector.c

CMakeFiles/poly.dir/src/Vector.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/poly.dir/src/Vector.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/yannvoytaa/duze/IPP-jw429685/src/Vector.c > CMakeFiles/poly.dir/src/Vector.c.i

CMakeFiles/poly.dir/src/Vector.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/poly.dir/src/Vector.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/yannvoytaa/duze/IPP-jw429685/src/Vector.c -o CMakeFiles/poly.dir/src/Vector.c.s

CMakeFiles/poly.dir/src/Stack.c.o: CMakeFiles/poly.dir/flags.make
CMakeFiles/poly.dir/src/Stack.c.o: ../src/Stack.c
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/poly.dir/src/Stack.c.o   -c /home/yannvoytaa/duze/IPP-jw429685/src/Stack.c

CMakeFiles/poly.dir/src/Stack.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/poly.dir/src/Stack.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/yannvoytaa/duze/IPP-jw429685/src/Stack.c > CMakeFiles/poly.dir/src/Stack.c.i

CMakeFiles/poly.dir/src/Stack.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/poly.dir/src/Stack.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/yannvoytaa/duze/IPP-jw429685/src/Stack.c -o CMakeFiles/poly.dir/src/Stack.c.s

CMakeFiles/poly.dir/src/CalcFunctions.c.o: CMakeFiles/poly.dir/flags.make
CMakeFiles/poly.dir/src/CalcFunctions.c.o: ../src/CalcFunctions.c
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/poly.dir/src/CalcFunctions.c.o   -c /home/yannvoytaa/duze/IPP-jw429685/src/CalcFunctions.c

CMakeFiles/poly.dir/src/CalcFunctions.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/poly.dir/src/CalcFunctions.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/yannvoytaa/duze/IPP-jw429685/src/CalcFunctions.c > CMakeFiles/poly.dir/src/CalcFunctions.c.i

CMakeFiles/poly.dir/src/CalcFunctions.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/poly.dir/src/CalcFunctions.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/yannvoytaa/duze/IPP-jw429685/src/CalcFunctions.c -o CMakeFiles/poly.dir/src/CalcFunctions.c.s

# Object files for target poly
poly_OBJECTS = \
"CMakeFiles/poly.dir/src/poly.c.o" \
"CMakeFiles/poly.dir/src/calc.c.o" \
"CMakeFiles/poly.dir/src/Vector.c.o" \
"CMakeFiles/poly.dir/src/Stack.c.o" \
"CMakeFiles/poly.dir/src/CalcFunctions.c.o"

# External object files for target poly
poly_EXTERNAL_OBJECTS =

poly: CMakeFiles/poly.dir/src/poly.c.o
poly: CMakeFiles/poly.dir/src/calc.c.o
poly: CMakeFiles/poly.dir/src/Vector.c.o
poly: CMakeFiles/poly.dir/src/Stack.c.o
poly: CMakeFiles/poly.dir/src/CalcFunctions.c.o
poly: CMakeFiles/poly.dir/build.make
poly: CMakeFiles/poly.dir/link.txt
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/poly.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/poly.dir/build: poly

.PHONY : CMakeFiles/poly.dir/build

CMakeFiles/poly.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/poly.dir/cmake_clean.cmake
.PHONY : CMakeFiles/poly.dir/clean

CMakeFiles/poly.dir/depend:
	cd /home/yannvoytaa/duze/IPP-jw429685/release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yannvoytaa/duze/IPP-jw429685 /home/yannvoytaa/duze/IPP-jw429685 /home/yannvoytaa/duze/IPP-jw429685/release /home/yannvoytaa/duze/IPP-jw429685/release /home/yannvoytaa/duze/IPP-jw429685/release/CMakeFiles/poly.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/poly.dir/depend

