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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.5.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.5.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/derya/Desktop/codes/h2sl

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/derya/Desktop/codes/h2sl/build

# Include any dependencies generated for this target.
include src/parser/CMakeFiles/h2sl-parser-demo.dir/depend.make

# Include the progress variables for this target.
include src/parser/CMakeFiles/h2sl-parser-demo.dir/progress.make

# Include the compile flags for this target's objects.
include src/parser/CMakeFiles/h2sl-parser-demo.dir/flags.make

src/parser/parser_demo_cmdline.c: ../src/parser/parser_demo.ggo
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/derya/Desktop/codes/h2sl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating parser_demo_cmdline.c, parser_demo_cmdline.h"
	cd /Users/derya/Desktop/codes/h2sl/build/src/parser && /usr/local/bin/gengetopt -i /Users/derya/Desktop/codes/h2sl/src/parser/parser_demo.ggo --file-name parser_demo_cmdline --output-dir=/Users/derya/Desktop/codes/h2sl/build/src/parser --unamed-opts

src/parser/parser_demo_cmdline.h: src/parser/parser_demo_cmdline.c
	@$(CMAKE_COMMAND) -E touch_nocreate src/parser/parser_demo_cmdline.h

src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo.cc.o: src/parser/CMakeFiles/h2sl-parser-demo.dir/flags.make
src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo.cc.o: ../src/parser/parser_demo.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/derya/Desktop/codes/h2sl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo.cc.o"
	cd /Users/derya/Desktop/codes/h2sl/build/src/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/h2sl-parser-demo.dir/parser_demo.cc.o -c /Users/derya/Desktop/codes/h2sl/src/parser/parser_demo.cc

src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/h2sl-parser-demo.dir/parser_demo.cc.i"
	cd /Users/derya/Desktop/codes/h2sl/build/src/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/derya/Desktop/codes/h2sl/src/parser/parser_demo.cc > CMakeFiles/h2sl-parser-demo.dir/parser_demo.cc.i

src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/h2sl-parser-demo.dir/parser_demo.cc.s"
	cd /Users/derya/Desktop/codes/h2sl/build/src/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/derya/Desktop/codes/h2sl/src/parser/parser_demo.cc -o CMakeFiles/h2sl-parser-demo.dir/parser_demo.cc.s

src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo.cc.o.requires:

.PHONY : src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo.cc.o.requires

src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo.cc.o.provides: src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo.cc.o.requires
	$(MAKE) -f src/parser/CMakeFiles/h2sl-parser-demo.dir/build.make src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo.cc.o.provides.build
.PHONY : src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo.cc.o.provides

src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo.cc.o.provides.build: src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo.cc.o


src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo_cmdline.c.o: src/parser/CMakeFiles/h2sl-parser-demo.dir/flags.make
src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo_cmdline.c.o: src/parser/parser_demo_cmdline.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/derya/Desktop/codes/h2sl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo_cmdline.c.o"
	cd /Users/derya/Desktop/codes/h2sl/build/src/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/h2sl-parser-demo.dir/parser_demo_cmdline.c.o   -c /Users/derya/Desktop/codes/h2sl/build/src/parser/parser_demo_cmdline.c

src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo_cmdline.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/h2sl-parser-demo.dir/parser_demo_cmdline.c.i"
	cd /Users/derya/Desktop/codes/h2sl/build/src/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/derya/Desktop/codes/h2sl/build/src/parser/parser_demo_cmdline.c > CMakeFiles/h2sl-parser-demo.dir/parser_demo_cmdline.c.i

src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo_cmdline.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/h2sl-parser-demo.dir/parser_demo_cmdline.c.s"
	cd /Users/derya/Desktop/codes/h2sl/build/src/parser && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/derya/Desktop/codes/h2sl/build/src/parser/parser_demo_cmdline.c -o CMakeFiles/h2sl-parser-demo.dir/parser_demo_cmdline.c.s

src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo_cmdline.c.o.requires:

.PHONY : src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo_cmdline.c.o.requires

src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo_cmdline.c.o.provides: src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo_cmdline.c.o.requires
	$(MAKE) -f src/parser/CMakeFiles/h2sl-parser-demo.dir/build.make src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo_cmdline.c.o.provides.build
.PHONY : src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo_cmdline.c.o.provides

src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo_cmdline.c.o.provides.build: src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo_cmdline.c.o


# Object files for target h2sl-parser-demo
h2sl__parser__demo_OBJECTS = \
"CMakeFiles/h2sl-parser-demo.dir/parser_demo.cc.o" \
"CMakeFiles/h2sl-parser-demo.dir/parser_demo_cmdline.c.o"

# External object files for target h2sl-parser-demo
h2sl__parser__demo_EXTERNAL_OBJECTS =

bin/h2sl-parser-demo: src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo.cc.o
bin/h2sl-parser-demo: src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo_cmdline.c.o
bin/h2sl-parser-demo: src/parser/CMakeFiles/h2sl-parser-demo.dir/build.make
bin/h2sl-parser-demo: lib/libh2sl-parser.a
bin/h2sl-parser-demo: lib/libh2sl-language.a
bin/h2sl-parser-demo: lib/libh2sl-symbol.a
bin/h2sl-parser-demo: lib/libh2sl-common.a
bin/h2sl-parser-demo: /usr/lib/libxml2.dylib
bin/h2sl-parser-demo: src/parser/CMakeFiles/h2sl-parser-demo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/derya/Desktop/codes/h2sl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable ../../bin/h2sl-parser-demo"
	cd /Users/derya/Desktop/codes/h2sl/build/src/parser && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/h2sl-parser-demo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/parser/CMakeFiles/h2sl-parser-demo.dir/build: bin/h2sl-parser-demo

.PHONY : src/parser/CMakeFiles/h2sl-parser-demo.dir/build

src/parser/CMakeFiles/h2sl-parser-demo.dir/requires: src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo.cc.o.requires
src/parser/CMakeFiles/h2sl-parser-demo.dir/requires: src/parser/CMakeFiles/h2sl-parser-demo.dir/parser_demo_cmdline.c.o.requires

.PHONY : src/parser/CMakeFiles/h2sl-parser-demo.dir/requires

src/parser/CMakeFiles/h2sl-parser-demo.dir/clean:
	cd /Users/derya/Desktop/codes/h2sl/build/src/parser && $(CMAKE_COMMAND) -P CMakeFiles/h2sl-parser-demo.dir/cmake_clean.cmake
.PHONY : src/parser/CMakeFiles/h2sl-parser-demo.dir/clean

src/parser/CMakeFiles/h2sl-parser-demo.dir/depend: src/parser/parser_demo_cmdline.c
src/parser/CMakeFiles/h2sl-parser-demo.dir/depend: src/parser/parser_demo_cmdline.h
	cd /Users/derya/Desktop/codes/h2sl/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/derya/Desktop/codes/h2sl /Users/derya/Desktop/codes/h2sl/src/parser /Users/derya/Desktop/codes/h2sl/build /Users/derya/Desktop/codes/h2sl/build/src/parser /Users/derya/Desktop/codes/h2sl/build/src/parser/CMakeFiles/h2sl-parser-demo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/parser/CMakeFiles/h2sl-parser-demo.dir/depend
