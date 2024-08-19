# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/mint/open/open

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mint/open/open/build

# Include any dependencies generated for this target.
include CMakeFiles/open_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/open_test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/open_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/open_test.dir/flags.make

CMakeFiles/open_test.dir/tests/open_test.cpp.o: CMakeFiles/open_test.dir/flags.make
CMakeFiles/open_test.dir/tests/open_test.cpp.o: /home/mint/open/open/tests/open_test.cpp
CMakeFiles/open_test.dir/tests/open_test.cpp.o: CMakeFiles/open_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/mint/open/open/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/open_test.dir/tests/open_test.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/open_test.dir/tests/open_test.cpp.o -MF CMakeFiles/open_test.dir/tests/open_test.cpp.o.d -o CMakeFiles/open_test.dir/tests/open_test.cpp.o -c /home/mint/open/open/tests/open_test.cpp

CMakeFiles/open_test.dir/tests/open_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/open_test.dir/tests/open_test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mint/open/open/tests/open_test.cpp > CMakeFiles/open_test.dir/tests/open_test.cpp.i

CMakeFiles/open_test.dir/tests/open_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/open_test.dir/tests/open_test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mint/open/open/tests/open_test.cpp -o CMakeFiles/open_test.dir/tests/open_test.cpp.s

# Object files for target open_test
open_test_OBJECTS = \
"CMakeFiles/open_test.dir/tests/open_test.cpp.o"

# External object files for target open_test
open_test_EXTERNAL_OBJECTS =

open_test: CMakeFiles/open_test.dir/tests/open_test.cpp.o
open_test: CMakeFiles/open_test.dir/build.make
open_test: lib/libgtest_main.a
open_test: lib/libgmock_main.a
open_test: libopen_lib.a
open_test: /home/mint/open/open/lib/libopencv_core.so
open_test: /home/mint/open/open/lib/libopencv_imgproc.so
open_test: /home/mint/open/open/lib/libopencv_highgui.so
open_test: /home/mint/open/open/lib/libopencv_imgcodecs.so
open_test: /home/mint/open/open/lib/libopencv_videoio.so
open_test: /home/mint/open/open/lib/libopencv_video.so
open_test: lib/libgmock.a
open_test: lib/libgtest.a
open_test: CMakeFiles/open_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/mint/open/open/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable open_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/open_test.dir/link.txt --verbose=$(VERBOSE)
	/usr/bin/cmake -D TEST_TARGET=open_test -D TEST_EXECUTABLE=/home/mint/open/open/build/open_test -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/home/mint/open/open/build -D TEST_EXTRA_ARGS= -D TEST_PROPERTIES= -D TEST_PREFIX= -D TEST_SUFFIX= -D TEST_FILTER= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=open_test_TESTS -D CTEST_FILE=/home/mint/open/open/build/open_test[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=5 -D TEST_XML_OUTPUT_DIR= -P /usr/share/cmake-3.28/Modules/GoogleTestAddTests.cmake

# Rule to build all files generated by this target.
CMakeFiles/open_test.dir/build: open_test
.PHONY : CMakeFiles/open_test.dir/build

CMakeFiles/open_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/open_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/open_test.dir/clean

CMakeFiles/open_test.dir/depend:
	cd /home/mint/open/open/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mint/open/open /home/mint/open/open /home/mint/open/open/build /home/mint/open/open/build /home/mint/open/open/build/CMakeFiles/open_test.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/open_test.dir/depend

