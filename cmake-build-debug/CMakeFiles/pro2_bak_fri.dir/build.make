# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/ax/Documents/Uvic/CSC 460/Project2/Demo/gitproject2/CSC460"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/ax/Documents/Uvic/CSC 460/Project2/Demo/gitproject2/CSC460/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/pro2_bak_fri.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/pro2_bak_fri.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/pro2_bak_fri.dir/flags.make

CMakeFiles/pro2_bak_fri.dir/active.c.o: CMakeFiles/pro2_bak_fri.dir/flags.make
CMakeFiles/pro2_bak_fri.dir/active.c.o: ../active.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/ax/Documents/Uvic/CSC 460/Project2/Demo/gitproject2/CSC460/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/pro2_bak_fri.dir/active.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/pro2_bak_fri.dir/active.c.o   -c "/Users/ax/Documents/Uvic/CSC 460/Project2/Demo/gitproject2/CSC460/active.c"

CMakeFiles/pro2_bak_fri.dir/active.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pro2_bak_fri.dir/active.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/ax/Documents/Uvic/CSC 460/Project2/Demo/gitproject2/CSC460/active.c" > CMakeFiles/pro2_bak_fri.dir/active.c.i

CMakeFiles/pro2_bak_fri.dir/active.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pro2_bak_fri.dir/active.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/ax/Documents/Uvic/CSC 460/Project2/Demo/gitproject2/CSC460/active.c" -o CMakeFiles/pro2_bak_fri.dir/active.c.s

CMakeFiles/pro2_bak_fri.dir/active.c.o.requires:

.PHONY : CMakeFiles/pro2_bak_fri.dir/active.c.o.requires

CMakeFiles/pro2_bak_fri.dir/active.c.o.provides: CMakeFiles/pro2_bak_fri.dir/active.c.o.requires
	$(MAKE) -f CMakeFiles/pro2_bak_fri.dir/build.make CMakeFiles/pro2_bak_fri.dir/active.c.o.provides.build
.PHONY : CMakeFiles/pro2_bak_fri.dir/active.c.o.provides

CMakeFiles/pro2_bak_fri.dir/active.c.o.provides.build: CMakeFiles/pro2_bak_fri.dir/active.c.o


CMakeFiles/pro2_bak_fri.dir/main.c.o: CMakeFiles/pro2_bak_fri.dir/flags.make
CMakeFiles/pro2_bak_fri.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/ax/Documents/Uvic/CSC 460/Project2/Demo/gitproject2/CSC460/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/pro2_bak_fri.dir/main.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/pro2_bak_fri.dir/main.c.o   -c "/Users/ax/Documents/Uvic/CSC 460/Project2/Demo/gitproject2/CSC460/main.c"

CMakeFiles/pro2_bak_fri.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pro2_bak_fri.dir/main.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/ax/Documents/Uvic/CSC 460/Project2/Demo/gitproject2/CSC460/main.c" > CMakeFiles/pro2_bak_fri.dir/main.c.i

CMakeFiles/pro2_bak_fri.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pro2_bak_fri.dir/main.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/ax/Documents/Uvic/CSC 460/Project2/Demo/gitproject2/CSC460/main.c" -o CMakeFiles/pro2_bak_fri.dir/main.c.s

CMakeFiles/pro2_bak_fri.dir/main.c.o.requires:

.PHONY : CMakeFiles/pro2_bak_fri.dir/main.c.o.requires

CMakeFiles/pro2_bak_fri.dir/main.c.o.provides: CMakeFiles/pro2_bak_fri.dir/main.c.o.requires
	$(MAKE) -f CMakeFiles/pro2_bak_fri.dir/build.make CMakeFiles/pro2_bak_fri.dir/main.c.o.provides.build
.PHONY : CMakeFiles/pro2_bak_fri.dir/main.c.o.provides

CMakeFiles/pro2_bak_fri.dir/main.c.o.provides.build: CMakeFiles/pro2_bak_fri.dir/main.c.o


CMakeFiles/pro2_bak_fri.dir/os.c.o: CMakeFiles/pro2_bak_fri.dir/flags.make
CMakeFiles/pro2_bak_fri.dir/os.c.o: ../os.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/ax/Documents/Uvic/CSC 460/Project2/Demo/gitproject2/CSC460/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/pro2_bak_fri.dir/os.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/pro2_bak_fri.dir/os.c.o   -c "/Users/ax/Documents/Uvic/CSC 460/Project2/Demo/gitproject2/CSC460/os.c"

CMakeFiles/pro2_bak_fri.dir/os.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pro2_bak_fri.dir/os.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/Users/ax/Documents/Uvic/CSC 460/Project2/Demo/gitproject2/CSC460/os.c" > CMakeFiles/pro2_bak_fri.dir/os.c.i

CMakeFiles/pro2_bak_fri.dir/os.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pro2_bak_fri.dir/os.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/Users/ax/Documents/Uvic/CSC 460/Project2/Demo/gitproject2/CSC460/os.c" -o CMakeFiles/pro2_bak_fri.dir/os.c.s

CMakeFiles/pro2_bak_fri.dir/os.c.o.requires:

.PHONY : CMakeFiles/pro2_bak_fri.dir/os.c.o.requires

CMakeFiles/pro2_bak_fri.dir/os.c.o.provides: CMakeFiles/pro2_bak_fri.dir/os.c.o.requires
	$(MAKE) -f CMakeFiles/pro2_bak_fri.dir/build.make CMakeFiles/pro2_bak_fri.dir/os.c.o.provides.build
.PHONY : CMakeFiles/pro2_bak_fri.dir/os.c.o.provides

CMakeFiles/pro2_bak_fri.dir/os.c.o.provides.build: CMakeFiles/pro2_bak_fri.dir/os.c.o


# Object files for target pro2_bak_fri
pro2_bak_fri_OBJECTS = \
"CMakeFiles/pro2_bak_fri.dir/active.c.o" \
"CMakeFiles/pro2_bak_fri.dir/main.c.o" \
"CMakeFiles/pro2_bak_fri.dir/os.c.o"

# External object files for target pro2_bak_fri
pro2_bak_fri_EXTERNAL_OBJECTS =

pro2_bak_fri: CMakeFiles/pro2_bak_fri.dir/active.c.o
pro2_bak_fri: CMakeFiles/pro2_bak_fri.dir/main.c.o
pro2_bak_fri: CMakeFiles/pro2_bak_fri.dir/os.c.o
pro2_bak_fri: CMakeFiles/pro2_bak_fri.dir/build.make
pro2_bak_fri: CMakeFiles/pro2_bak_fri.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/ax/Documents/Uvic/CSC 460/Project2/Demo/gitproject2/CSC460/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable pro2_bak_fri"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pro2_bak_fri.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/pro2_bak_fri.dir/build: pro2_bak_fri

.PHONY : CMakeFiles/pro2_bak_fri.dir/build

CMakeFiles/pro2_bak_fri.dir/requires: CMakeFiles/pro2_bak_fri.dir/active.c.o.requires
CMakeFiles/pro2_bak_fri.dir/requires: CMakeFiles/pro2_bak_fri.dir/main.c.o.requires
CMakeFiles/pro2_bak_fri.dir/requires: CMakeFiles/pro2_bak_fri.dir/os.c.o.requires

.PHONY : CMakeFiles/pro2_bak_fri.dir/requires

CMakeFiles/pro2_bak_fri.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/pro2_bak_fri.dir/cmake_clean.cmake
.PHONY : CMakeFiles/pro2_bak_fri.dir/clean

CMakeFiles/pro2_bak_fri.dir/depend:
	cd "/Users/ax/Documents/Uvic/CSC 460/Project2/Demo/gitproject2/CSC460/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/ax/Documents/Uvic/CSC 460/Project2/Demo/gitproject2/CSC460" "/Users/ax/Documents/Uvic/CSC 460/Project2/Demo/gitproject2/CSC460" "/Users/ax/Documents/Uvic/CSC 460/Project2/Demo/gitproject2/CSC460/cmake-build-debug" "/Users/ax/Documents/Uvic/CSC 460/Project2/Demo/gitproject2/CSC460/cmake-build-debug" "/Users/ax/Documents/Uvic/CSC 460/Project2/Demo/gitproject2/CSC460/cmake-build-debug/CMakeFiles/pro2_bak_fri.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/pro2_bak_fri.dir/depend

