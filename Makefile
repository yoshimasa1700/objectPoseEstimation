# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

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
<<<<<<< HEAD
CMAKE_SOURCE_DIR = /Users/masahiko/Programs/objectPoseEstimation

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/masahiko/Programs/objectPoseEstimation
=======
CMAKE_SOURCE_DIR = /home/masahiko/Program/objectPoseEstimation

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/masahiko/Program/objectPoseEstimation
>>>>>>> origin/master

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running interactive CMake command-line interface..."
	/usr/bin/cmake -i .
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
<<<<<<< HEAD
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/masahiko/Programs/objectPoseEstimation/CMakeFiles /Users/masahiko/Programs/objectPoseEstimation/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /Users/masahiko/Programs/objectPoseEstimation/CMakeFiles 0
=======
	$(CMAKE_COMMAND) -E cmake_progress_start /home/masahiko/Program/objectPoseEstimation/CMakeFiles /home/masahiko/Program/objectPoseEstimation/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/masahiko/Program/objectPoseEstimation/CMakeFiles 0
>>>>>>> origin/master
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named learning

# Build rule for target.
learning: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 learning
.PHONY : learning

# fast build rule for target.
learning/fast:
	$(MAKE) -f CMakeFiles/learning.dir/build.make CMakeFiles/learning.dir/build
.PHONY : learning/fast

#=============================================================================
# Target rules for targets named objectPoseEstimation

# Build rule for target.
objectPoseEstimation: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 objectPoseEstimation
.PHONY : objectPoseEstimation

# fast build rule for target.
objectPoseEstimation/fast:
	$(MAKE) -f CMakeFiles/objectPoseEstimation.dir/build.make CMakeFiles/objectPoseEstimation.dir/build
.PHONY : objectPoseEstimation/fast

CRForest.o: CRForest.cpp.o
.PHONY : CRForest.o

# target to build an object file
CRForest.cpp.o:
	$(MAKE) -f CMakeFiles/learning.dir/build.make CMakeFiles/learning.dir/CRForest.cpp.o
	$(MAKE) -f CMakeFiles/objectPoseEstimation.dir/build.make CMakeFiles/objectPoseEstimation.dir/CRForest.cpp.o
.PHONY : CRForest.cpp.o

CRForest.i: CRForest.cpp.i
.PHONY : CRForest.i

# target to preprocess a source file
CRForest.cpp.i:
	$(MAKE) -f CMakeFiles/learning.dir/build.make CMakeFiles/learning.dir/CRForest.cpp.i
	$(MAKE) -f CMakeFiles/objectPoseEstimation.dir/build.make CMakeFiles/objectPoseEstimation.dir/CRForest.cpp.i
.PHONY : CRForest.cpp.i

CRForest.s: CRForest.cpp.s
.PHONY : CRForest.s

# target to generate assembly for a file
CRForest.cpp.s:
	$(MAKE) -f CMakeFiles/learning.dir/build.make CMakeFiles/learning.dir/CRForest.cpp.s
	$(MAKE) -f CMakeFiles/objectPoseEstimation.dir/build.make CMakeFiles/objectPoseEstimation.dir/CRForest.cpp.s
.PHONY : CRForest.cpp.s

CRTree.o: CRTree.cpp.o
.PHONY : CRTree.o

# target to build an object file
CRTree.cpp.o:
	$(MAKE) -f CMakeFiles/learning.dir/build.make CMakeFiles/learning.dir/CRTree.cpp.o
	$(MAKE) -f CMakeFiles/objectPoseEstimation.dir/build.make CMakeFiles/objectPoseEstimation.dir/CRTree.cpp.o
.PHONY : CRTree.cpp.o

CRTree.i: CRTree.cpp.i
.PHONY : CRTree.i

# target to preprocess a source file
CRTree.cpp.i:
	$(MAKE) -f CMakeFiles/learning.dir/build.make CMakeFiles/learning.dir/CRTree.cpp.i
	$(MAKE) -f CMakeFiles/objectPoseEstimation.dir/build.make CMakeFiles/objectPoseEstimation.dir/CRTree.cpp.i
.PHONY : CRTree.cpp.i

CRTree.s: CRTree.cpp.s
.PHONY : CRTree.s

# target to generate assembly for a file
CRTree.cpp.s:
	$(MAKE) -f CMakeFiles/learning.dir/build.make CMakeFiles/learning.dir/CRTree.cpp.s
	$(MAKE) -f CMakeFiles/objectPoseEstimation.dir/build.make CMakeFiles/objectPoseEstimation.dir/CRTree.cpp.s
.PHONY : CRTree.cpp.s

HoG.o: HoG.cpp.o
.PHONY : HoG.o

# target to build an object file
HoG.cpp.o:
	$(MAKE) -f CMakeFiles/learning.dir/build.make CMakeFiles/learning.dir/HoG.cpp.o
	$(MAKE) -f CMakeFiles/objectPoseEstimation.dir/build.make CMakeFiles/objectPoseEstimation.dir/HoG.cpp.o
.PHONY : HoG.cpp.o

HoG.i: HoG.cpp.i
.PHONY : HoG.i

# target to preprocess a source file
HoG.cpp.i:
	$(MAKE) -f CMakeFiles/learning.dir/build.make CMakeFiles/learning.dir/HoG.cpp.i
	$(MAKE) -f CMakeFiles/objectPoseEstimation.dir/build.make CMakeFiles/objectPoseEstimation.dir/HoG.cpp.i
.PHONY : HoG.cpp.i

HoG.s: HoG.cpp.s
.PHONY : HoG.s

# target to generate assembly for a file
HoG.cpp.s:
	$(MAKE) -f CMakeFiles/learning.dir/build.make CMakeFiles/learning.dir/HoG.cpp.s
	$(MAKE) -f CMakeFiles/objectPoseEstimation.dir/build.make CMakeFiles/objectPoseEstimation.dir/HoG.cpp.s
.PHONY : HoG.cpp.s

learning.o: learning.cpp.o
.PHONY : learning.o

# target to build an object file
learning.cpp.o:
	$(MAKE) -f CMakeFiles/learning.dir/build.make CMakeFiles/learning.dir/learning.cpp.o
.PHONY : learning.cpp.o

learning.i: learning.cpp.i
.PHONY : learning.i

# target to preprocess a source file
learning.cpp.i:
	$(MAKE) -f CMakeFiles/learning.dir/build.make CMakeFiles/learning.dir/learning.cpp.i
.PHONY : learning.cpp.i

learning.s: learning.cpp.s
.PHONY : learning.s

# target to generate assembly for a file
learning.cpp.s:
	$(MAKE) -f CMakeFiles/learning.dir/build.make CMakeFiles/learning.dir/learning.cpp.s
.PHONY : learning.cpp.s

objectPoseEstimation.o: objectPoseEstimation.cpp.o
.PHONY : objectPoseEstimation.o

# target to build an object file
objectPoseEstimation.cpp.o:
	$(MAKE) -f CMakeFiles/objectPoseEstimation.dir/build.make CMakeFiles/objectPoseEstimation.dir/objectPoseEstimation.cpp.o
.PHONY : objectPoseEstimation.cpp.o

objectPoseEstimation.i: objectPoseEstimation.cpp.i
.PHONY : objectPoseEstimation.i

# target to preprocess a source file
objectPoseEstimation.cpp.i:
	$(MAKE) -f CMakeFiles/objectPoseEstimation.dir/build.make CMakeFiles/objectPoseEstimation.dir/objectPoseEstimation.cpp.i
.PHONY : objectPoseEstimation.cpp.i

objectPoseEstimation.s: objectPoseEstimation.cpp.s
.PHONY : objectPoseEstimation.s

# target to generate assembly for a file
objectPoseEstimation.cpp.s:
	$(MAKE) -f CMakeFiles/objectPoseEstimation.dir/build.make CMakeFiles/objectPoseEstimation.dir/objectPoseEstimation.cpp.s
.PHONY : objectPoseEstimation.cpp.s

util.o: util.cpp.o
.PHONY : util.o

# target to build an object file
util.cpp.o:
	$(MAKE) -f CMakeFiles/learning.dir/build.make CMakeFiles/learning.dir/util.cpp.o
	$(MAKE) -f CMakeFiles/objectPoseEstimation.dir/build.make CMakeFiles/objectPoseEstimation.dir/util.cpp.o
.PHONY : util.cpp.o

util.i: util.cpp.i
.PHONY : util.i

# target to preprocess a source file
util.cpp.i:
	$(MAKE) -f CMakeFiles/learning.dir/build.make CMakeFiles/learning.dir/util.cpp.i
	$(MAKE) -f CMakeFiles/objectPoseEstimation.dir/build.make CMakeFiles/objectPoseEstimation.dir/util.cpp.i
.PHONY : util.cpp.i

util.s: util.cpp.s
.PHONY : util.s

# target to generate assembly for a file
util.cpp.s:
	$(MAKE) -f CMakeFiles/learning.dir/build.make CMakeFiles/learning.dir/util.cpp.s
	$(MAKE) -f CMakeFiles/objectPoseEstimation.dir/build.make CMakeFiles/objectPoseEstimation.dir/util.cpp.s
.PHONY : util.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... learning"
	@echo "... objectPoseEstimation"
	@echo "... rebuild_cache"
	@echo "... CRForest.o"
	@echo "... CRForest.i"
	@echo "... CRForest.s"
	@echo "... CRTree.o"
	@echo "... CRTree.i"
	@echo "... CRTree.s"
	@echo "... HoG.o"
	@echo "... HoG.i"
	@echo "... HoG.s"
	@echo "... learning.o"
	@echo "... learning.i"
	@echo "... learning.s"
	@echo "... objectPoseEstimation.o"
	@echo "... objectPoseEstimation.i"
	@echo "... objectPoseEstimation.s"
	@echo "... util.o"
	@echo "... util.i"
	@echo "... util.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

