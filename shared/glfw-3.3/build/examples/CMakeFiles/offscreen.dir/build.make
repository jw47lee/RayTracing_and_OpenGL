# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /org/cs/u1/jw47lee/cs488/shared/glfw-3.3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /org/cs/u1/jw47lee/cs488/shared/glfw-3.3/build

# Include any dependencies generated for this target.
include examples/CMakeFiles/offscreen.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/offscreen.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/offscreen.dir/flags.make

examples/CMakeFiles/offscreen.dir/offscreen.c.o: examples/CMakeFiles/offscreen.dir/flags.make
examples/CMakeFiles/offscreen.dir/offscreen.c.o: ../examples/offscreen.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/org/cs/u1/jw47lee/cs488/shared/glfw-3.3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object examples/CMakeFiles/offscreen.dir/offscreen.c.o"
	cd /org/cs/u1/jw47lee/cs488/shared/glfw-3.3/build/examples && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/offscreen.dir/offscreen.c.o   -c /org/cs/u1/jw47lee/cs488/shared/glfw-3.3/examples/offscreen.c

examples/CMakeFiles/offscreen.dir/offscreen.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/offscreen.dir/offscreen.c.i"
	cd /org/cs/u1/jw47lee/cs488/shared/glfw-3.3/build/examples && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /org/cs/u1/jw47lee/cs488/shared/glfw-3.3/examples/offscreen.c > CMakeFiles/offscreen.dir/offscreen.c.i

examples/CMakeFiles/offscreen.dir/offscreen.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/offscreen.dir/offscreen.c.s"
	cd /org/cs/u1/jw47lee/cs488/shared/glfw-3.3/build/examples && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /org/cs/u1/jw47lee/cs488/shared/glfw-3.3/examples/offscreen.c -o CMakeFiles/offscreen.dir/offscreen.c.s

examples/CMakeFiles/offscreen.dir/offscreen.c.o.requires:

.PHONY : examples/CMakeFiles/offscreen.dir/offscreen.c.o.requires

examples/CMakeFiles/offscreen.dir/offscreen.c.o.provides: examples/CMakeFiles/offscreen.dir/offscreen.c.o.requires
	$(MAKE) -f examples/CMakeFiles/offscreen.dir/build.make examples/CMakeFiles/offscreen.dir/offscreen.c.o.provides.build
.PHONY : examples/CMakeFiles/offscreen.dir/offscreen.c.o.provides

examples/CMakeFiles/offscreen.dir/offscreen.c.o.provides.build: examples/CMakeFiles/offscreen.dir/offscreen.c.o


examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o: examples/CMakeFiles/offscreen.dir/flags.make
examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o: ../deps/glad_gl.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/org/cs/u1/jw47lee/cs488/shared/glfw-3.3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o"
	cd /org/cs/u1/jw47lee/cs488/shared/glfw-3.3/build/examples && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o   -c /org/cs/u1/jw47lee/cs488/shared/glfw-3.3/deps/glad_gl.c

examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/offscreen.dir/__/deps/glad_gl.c.i"
	cd /org/cs/u1/jw47lee/cs488/shared/glfw-3.3/build/examples && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /org/cs/u1/jw47lee/cs488/shared/glfw-3.3/deps/glad_gl.c > CMakeFiles/offscreen.dir/__/deps/glad_gl.c.i

examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/offscreen.dir/__/deps/glad_gl.c.s"
	cd /org/cs/u1/jw47lee/cs488/shared/glfw-3.3/build/examples && /usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /org/cs/u1/jw47lee/cs488/shared/glfw-3.3/deps/glad_gl.c -o CMakeFiles/offscreen.dir/__/deps/glad_gl.c.s

examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o.requires:

.PHONY : examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o.requires

examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o.provides: examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o.requires
	$(MAKE) -f examples/CMakeFiles/offscreen.dir/build.make examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o.provides.build
.PHONY : examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o.provides

examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o.provides.build: examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o


# Object files for target offscreen
offscreen_OBJECTS = \
"CMakeFiles/offscreen.dir/offscreen.c.o" \
"CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o"

# External object files for target offscreen
offscreen_EXTERNAL_OBJECTS =

examples/offscreen: examples/CMakeFiles/offscreen.dir/offscreen.c.o
examples/offscreen: examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o
examples/offscreen: examples/CMakeFiles/offscreen.dir/build.make
examples/offscreen: src/libglfw3.a
examples/offscreen: /usr/lib/x86_64-linux-gnu/libm.so
examples/offscreen: /usr/lib/x86_64-linux-gnu/librt.so
examples/offscreen: /usr/lib/x86_64-linux-gnu/libm.so
examples/offscreen: /usr/lib/x86_64-linux-gnu/libX11.so
examples/offscreen: examples/CMakeFiles/offscreen.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/org/cs/u1/jw47lee/cs488/shared/glfw-3.3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable offscreen"
	cd /org/cs/u1/jw47lee/cs488/shared/glfw-3.3/build/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/offscreen.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/offscreen.dir/build: examples/offscreen

.PHONY : examples/CMakeFiles/offscreen.dir/build

examples/CMakeFiles/offscreen.dir/requires: examples/CMakeFiles/offscreen.dir/offscreen.c.o.requires
examples/CMakeFiles/offscreen.dir/requires: examples/CMakeFiles/offscreen.dir/__/deps/glad_gl.c.o.requires

.PHONY : examples/CMakeFiles/offscreen.dir/requires

examples/CMakeFiles/offscreen.dir/clean:
	cd /org/cs/u1/jw47lee/cs488/shared/glfw-3.3/build/examples && $(CMAKE_COMMAND) -P CMakeFiles/offscreen.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/offscreen.dir/clean

examples/CMakeFiles/offscreen.dir/depend:
	cd /org/cs/u1/jw47lee/cs488/shared/glfw-3.3/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /org/cs/u1/jw47lee/cs488/shared/glfw-3.3 /org/cs/u1/jw47lee/cs488/shared/glfw-3.3/examples /org/cs/u1/jw47lee/cs488/shared/glfw-3.3/build /org/cs/u1/jw47lee/cs488/shared/glfw-3.3/build/examples /org/cs/u1/jw47lee/cs488/shared/glfw-3.3/build/examples/CMakeFiles/offscreen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/offscreen.dir/depend

