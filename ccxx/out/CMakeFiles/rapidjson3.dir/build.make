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
CMAKE_SOURCE_DIR = /home/peak/develop/git/LearnMore/test/ccxx

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/peak/develop/git/LearnMore/test/ccxx/out

# Include any dependencies generated for this target.
include CMakeFiles/rapidjson3.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/rapidjson3.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rapidjson3.dir/flags.make

CMakeFiles/rapidjson3.dir/rapidjson3.cpp.o: CMakeFiles/rapidjson3.dir/flags.make
CMakeFiles/rapidjson3.dir/rapidjson3.cpp.o: ../rapidjson3.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/peak/develop/git/LearnMore/test/ccxx/out/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/rapidjson3.dir/rapidjson3.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/rapidjson3.dir/rapidjson3.cpp.o -c /home/peak/develop/git/LearnMore/test/ccxx/rapidjson3.cpp

CMakeFiles/rapidjson3.dir/rapidjson3.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rapidjson3.dir/rapidjson3.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/peak/develop/git/LearnMore/test/ccxx/rapidjson3.cpp > CMakeFiles/rapidjson3.dir/rapidjson3.cpp.i

CMakeFiles/rapidjson3.dir/rapidjson3.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rapidjson3.dir/rapidjson3.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/peak/develop/git/LearnMore/test/ccxx/rapidjson3.cpp -o CMakeFiles/rapidjson3.dir/rapidjson3.cpp.s

CMakeFiles/rapidjson3.dir/rapidjson3.cpp.o.requires:

.PHONY : CMakeFiles/rapidjson3.dir/rapidjson3.cpp.o.requires

CMakeFiles/rapidjson3.dir/rapidjson3.cpp.o.provides: CMakeFiles/rapidjson3.dir/rapidjson3.cpp.o.requires
	$(MAKE) -f CMakeFiles/rapidjson3.dir/build.make CMakeFiles/rapidjson3.dir/rapidjson3.cpp.o.provides.build
.PHONY : CMakeFiles/rapidjson3.dir/rapidjson3.cpp.o.provides

CMakeFiles/rapidjson3.dir/rapidjson3.cpp.o.provides.build: CMakeFiles/rapidjson3.dir/rapidjson3.cpp.o


# Object files for target rapidjson3
rapidjson3_OBJECTS = \
"CMakeFiles/rapidjson3.dir/rapidjson3.cpp.o"

# External object files for target rapidjson3
rapidjson3_EXTERNAL_OBJECTS =

rapidjson3: CMakeFiles/rapidjson3.dir/rapidjson3.cpp.o
rapidjson3: CMakeFiles/rapidjson3.dir/build.make
rapidjson3: CMakeFiles/rapidjson3.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/peak/develop/git/LearnMore/test/ccxx/out/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable rapidjson3"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rapidjson3.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rapidjson3.dir/build: rapidjson3

.PHONY : CMakeFiles/rapidjson3.dir/build

CMakeFiles/rapidjson3.dir/requires: CMakeFiles/rapidjson3.dir/rapidjson3.cpp.o.requires

.PHONY : CMakeFiles/rapidjson3.dir/requires

CMakeFiles/rapidjson3.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rapidjson3.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rapidjson3.dir/clean

CMakeFiles/rapidjson3.dir/depend:
	cd /home/peak/develop/git/LearnMore/test/ccxx/out && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/peak/develop/git/LearnMore/test/ccxx /home/peak/develop/git/LearnMore/test/ccxx /home/peak/develop/git/LearnMore/test/ccxx/out /home/peak/develop/git/LearnMore/test/ccxx/out /home/peak/develop/git/LearnMore/test/ccxx/out/CMakeFiles/rapidjson3.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/rapidjson3.dir/depend
