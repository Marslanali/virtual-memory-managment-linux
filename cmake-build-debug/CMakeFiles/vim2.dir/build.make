# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /opt/clion-2019.3.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion-2019.3.4/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/arslan/CLionProjects/virtual-memory-managment-linux

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/arslan/CLionProjects/virtual-memory-managment-linux/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/vim2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/vim2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/vim2.dir/flags.make

CMakeFiles/vim2.dir/vmm2.c.o: CMakeFiles/vim2.dir/flags.make
CMakeFiles/vim2.dir/vmm2.c.o: ../vmm2.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/arslan/CLionProjects/virtual-memory-managment-linux/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/vim2.dir/vmm2.c.o"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/vim2.dir/vmm2.c.o   -c /home/arslan/CLionProjects/virtual-memory-managment-linux/vmm2.c

CMakeFiles/vim2.dir/vmm2.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/vim2.dir/vmm2.c.i"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/arslan/CLionProjects/virtual-memory-managment-linux/vmm2.c > CMakeFiles/vim2.dir/vmm2.c.i

CMakeFiles/vim2.dir/vmm2.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/vim2.dir/vmm2.c.s"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/arslan/CLionProjects/virtual-memory-managment-linux/vmm2.c -o CMakeFiles/vim2.dir/vmm2.c.s

# Object files for target vim2
vim2_OBJECTS = \
"CMakeFiles/vim2.dir/vmm2.c.o"

# External object files for target vim2
vim2_EXTERNAL_OBJECTS =

vim2: CMakeFiles/vim2.dir/vmm2.c.o
vim2: CMakeFiles/vim2.dir/build.make
vim2: CMakeFiles/vim2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/arslan/CLionProjects/virtual-memory-managment-linux/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable vim2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vim2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/vim2.dir/build: vim2

.PHONY : CMakeFiles/vim2.dir/build

CMakeFiles/vim2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/vim2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/vim2.dir/clean

CMakeFiles/vim2.dir/depend:
	cd /home/arslan/CLionProjects/virtual-memory-managment-linux/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/arslan/CLionProjects/virtual-memory-managment-linux /home/arslan/CLionProjects/virtual-memory-managment-linux /home/arslan/CLionProjects/virtual-memory-managment-linux/cmake-build-debug /home/arslan/CLionProjects/virtual-memory-managment-linux/cmake-build-debug /home/arslan/CLionProjects/virtual-memory-managment-linux/cmake-build-debug/CMakeFiles/vim2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/vim2.dir/depend

