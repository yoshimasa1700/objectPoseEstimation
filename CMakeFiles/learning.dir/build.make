# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = "/Applications/CMake 2.8-9.app/Contents/bin/cmake"

# The command to remove a file.
RM = "/Applications/CMake 2.8-9.app/Contents/bin/cmake" -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = "/Applications/CMake 2.8-9.app/Contents/bin/ccmake"

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/masahiko/Programs/objectPoseEstimation

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/masahiko/Programs/objectPoseEstimation

# Include any dependencies generated for this target.
include CMakeFiles/learning.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/learning.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/learning.dir/flags.make

CMakeFiles/learning.dir/learning.cpp.o: CMakeFiles/learning.dir/flags.make
CMakeFiles/learning.dir/learning.cpp.o: learning.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/masahiko/Programs/objectPoseEstimation/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/learning.dir/learning.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/learning.dir/learning.cpp.o -c /Users/masahiko/Programs/objectPoseEstimation/learning.cpp

CMakeFiles/learning.dir/learning.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/learning.dir/learning.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/masahiko/Programs/objectPoseEstimation/learning.cpp > CMakeFiles/learning.dir/learning.cpp.i

CMakeFiles/learning.dir/learning.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/learning.dir/learning.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/masahiko/Programs/objectPoseEstimation/learning.cpp -o CMakeFiles/learning.dir/learning.cpp.s

CMakeFiles/learning.dir/learning.cpp.o.requires:
.PHONY : CMakeFiles/learning.dir/learning.cpp.o.requires

CMakeFiles/learning.dir/learning.cpp.o.provides: CMakeFiles/learning.dir/learning.cpp.o.requires
	$(MAKE) -f CMakeFiles/learning.dir/build.make CMakeFiles/learning.dir/learning.cpp.o.provides.build
.PHONY : CMakeFiles/learning.dir/learning.cpp.o.provides

CMakeFiles/learning.dir/learning.cpp.o.provides.build: CMakeFiles/learning.dir/learning.cpp.o

CMakeFiles/learning.dir/loadConfig.cpp.o: CMakeFiles/learning.dir/flags.make
CMakeFiles/learning.dir/loadConfig.cpp.o: loadConfig.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /Users/masahiko/Programs/objectPoseEstimation/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/learning.dir/loadConfig.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/learning.dir/loadConfig.cpp.o -c /Users/masahiko/Programs/objectPoseEstimation/loadConfig.cpp

CMakeFiles/learning.dir/loadConfig.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/learning.dir/loadConfig.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /Users/masahiko/Programs/objectPoseEstimation/loadConfig.cpp > CMakeFiles/learning.dir/loadConfig.cpp.i

CMakeFiles/learning.dir/loadConfig.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/learning.dir/loadConfig.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /Users/masahiko/Programs/objectPoseEstimation/loadConfig.cpp -o CMakeFiles/learning.dir/loadConfig.cpp.s

CMakeFiles/learning.dir/loadConfig.cpp.o.requires:
.PHONY : CMakeFiles/learning.dir/loadConfig.cpp.o.requires

CMakeFiles/learning.dir/loadConfig.cpp.o.provides: CMakeFiles/learning.dir/loadConfig.cpp.o.requires
	$(MAKE) -f CMakeFiles/learning.dir/build.make CMakeFiles/learning.dir/loadConfig.cpp.o.provides.build
.PHONY : CMakeFiles/learning.dir/loadConfig.cpp.o.provides

CMakeFiles/learning.dir/loadConfig.cpp.o.provides.build: CMakeFiles/learning.dir/loadConfig.cpp.o

# Object files for target learning
learning_OBJECTS = \
"CMakeFiles/learning.dir/learning.cpp.o" \
"CMakeFiles/learning.dir/loadConfig.cpp.o"

# External object files for target learning
learning_EXTERNAL_OBJECTS =

learning-1.0.0: CMakeFiles/learning.dir/learning.cpp.o
learning-1.0.0: CMakeFiles/learning.dir/loadConfig.cpp.o
learning-1.0.0: CMakeFiles/learning.dir/build.make
learning-1.0.0: /usr/local/lib/libopencv_calib3d.dylib
learning-1.0.0: /usr/local/lib/libopencv_contrib.dylib
learning-1.0.0: /usr/local/lib/libopencv_core.dylib
learning-1.0.0: /usr/local/lib/libopencv_features2d.dylib
learning-1.0.0: /usr/local/lib/libopencv_flann.dylib
learning-1.0.0: /usr/local/lib/libopencv_gpu.dylib
learning-1.0.0: /usr/local/lib/libopencv_highgui.dylib
learning-1.0.0: /usr/local/lib/libopencv_imgproc.dylib
learning-1.0.0: /usr/local/lib/libopencv_legacy.dylib
learning-1.0.0: /usr/local/lib/libopencv_ml.dylib
learning-1.0.0: /usr/local/lib/libopencv_nonfree.dylib
learning-1.0.0: /usr/local/lib/libopencv_objdetect.dylib
learning-1.0.0: /usr/local/lib/libopencv_photo.dylib
learning-1.0.0: /usr/local/lib/libopencv_stitching.dylib
learning-1.0.0: /usr/local/lib/libopencv_ts.dylib
learning-1.0.0: /usr/local/lib/libopencv_video.dylib
learning-1.0.0: /usr/local/lib/libopencv_videostab.dylib
learning-1.0.0: CMakeFiles/learning.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable learning"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/learning.dir/link.txt --verbose=$(VERBOSE)
	$(CMAKE_COMMAND) -E cmake_symlink_executable learning-1.0.0 learning

learning: learning-1.0.0

# Rule to build all files generated by this target.
CMakeFiles/learning.dir/build: learning
.PHONY : CMakeFiles/learning.dir/build

CMakeFiles/learning.dir/requires: CMakeFiles/learning.dir/learning.cpp.o.requires
CMakeFiles/learning.dir/requires: CMakeFiles/learning.dir/loadConfig.cpp.o.requires
.PHONY : CMakeFiles/learning.dir/requires

CMakeFiles/learning.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/learning.dir/cmake_clean.cmake
.PHONY : CMakeFiles/learning.dir/clean

CMakeFiles/learning.dir/depend:
	cd /Users/masahiko/Programs/objectPoseEstimation && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/masahiko/Programs/objectPoseEstimation /Users/masahiko/Programs/objectPoseEstimation /Users/masahiko/Programs/objectPoseEstimation /Users/masahiko/Programs/objectPoseEstimation /Users/masahiko/Programs/objectPoseEstimation/CMakeFiles/learning.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/learning.dir/depend

