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
include CMakeFiles/shared_ptr.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/shared_ptr.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/shared_ptr.dir/flags.make

CMakeFiles/shared_ptr.dir/shared_ptr.cpp.o: CMakeFiles/shared_ptr.dir/flags.make
CMakeFiles/shared_ptr.dir/shared_ptr.cpp.o: ../shared_ptr.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/peak/develop/git/LearnMore/test/ccxx/out/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/shared_ptr.dir/shared_ptr.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/shared_ptr.dir/shared_ptr.cpp.o -c /home/peak/develop/git/LearnMore/test/ccxx/shared_ptr.cpp

CMakeFiles/shared_ptr.dir/shared_ptr.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/shared_ptr.dir/shared_ptr.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/peak/develop/git/LearnMore/test/ccxx/shared_ptr.cpp > CMakeFiles/shared_ptr.dir/shared_ptr.cpp.i

CMakeFiles/shared_ptr.dir/shared_ptr.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/shared_ptr.dir/shared_ptr.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/peak/develop/git/LearnMore/test/ccxx/shared_ptr.cpp -o CMakeFiles/shared_ptr.dir/shared_ptr.cpp.s

CMakeFiles/shared_ptr.dir/shared_ptr.cpp.o.requires:

.PHONY : CMakeFiles/shared_ptr.dir/shared_ptr.cpp.o.requires

CMakeFiles/shared_ptr.dir/shared_ptr.cpp.o.provides: CMakeFiles/shared_ptr.dir/shared_ptr.cpp.o.requires
	$(MAKE) -f CMakeFiles/shared_ptr.dir/build.make CMakeFiles/shared_ptr.dir/shared_ptr.cpp.o.provides.build
.PHONY : CMakeFiles/shared_ptr.dir/shared_ptr.cpp.o.provides

CMakeFiles/shared_ptr.dir/shared_ptr.cpp.o.provides.build: CMakeFiles/shared_ptr.dir/shared_ptr.cpp.o


# Object files for target shared_ptr
shared_ptr_OBJECTS = \
"CMakeFiles/shared_ptr.dir/shared_ptr.cpp.o"

# External object files for target shared_ptr
shared_ptr_EXTERNAL_OBJECTS =

shared_ptr: CMakeFiles/shared_ptr.dir/shared_ptr.cpp.o
shared_ptr: CMakeFiles/shared_ptr.dir/build.make
shared_ptr: CMakeFiles/shared_ptr.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/peak/develop/git/LearnMore/test/ccxx/out/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable shared_ptr"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/shared_ptr.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/shared_ptr.dir/build: shared_ptr

.PHONY : CMakeFiles/shared_ptr.dir/build

CMakeFiles/shared_ptr.dir/requires: CMakeFiles/shared_ptr.dir/shared_ptr.cpp.o.requires

.PHONY : CMakeFiles/shared_ptr.dir/requires

CMakeFiles/shared_ptr.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/shared_ptr.dir/cmake_clean.cmake
.PHONY : CMakeFiles/shared_ptr.dir/clean

CMakeFiles/shared_ptr.dir/depend:
	cd /home/peak/develop/git/LearnMore/test/ccxx/out && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/peak/develop/git/LearnMore/test/ccxx /home/peak/develop/git/LearnMore/test/ccxx /home/peak/develop/git/LearnMore/test/ccxx/out /home/peak/develop/git/LearnMore/test/ccxx/out /home/peak/develop/git/LearnMore/test/ccxx/out/CMakeFiles/shared_ptr.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/shared_ptr.dir/depend

