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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.28.3/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.28.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/eeleexx/This_is_fine_client/this-is-fine-client

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/eeleexx/This_is_fine_client/this-is-fine-client/build

# Utility rule file for gmock_autogen.

# Include any custom commands dependencies for this target.
include extern/googletest/googlemock/CMakeFiles/gmock_autogen.dir/compiler_depend.make

# Include the progress variables for this target.
include extern/googletest/googlemock/CMakeFiles/gmock_autogen.dir/progress.make

extern/googletest/googlemock/CMakeFiles/gmock_autogen: extern/googletest/googlemock/gmock_autogen/timestamp

extern/googletest/googlemock/gmock_autogen/timestamp: /opt/homebrew/share/qt/libexec/moc
extern/googletest/googlemock/gmock_autogen/timestamp: /opt/homebrew/share/qt/libexec/uic
extern/googletest/googlemock/gmock_autogen/timestamp: extern/googletest/googlemock/CMakeFiles/gmock_autogen.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/Users/eeleexx/This_is_fine_client/this-is-fine-client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target gmock"
	cd /Users/eeleexx/This_is_fine_client/this-is-fine-client/build/extern/googletest/googlemock && /opt/homebrew/Cellar/cmake/3.28.3/bin/cmake -E cmake_autogen /Users/eeleexx/This_is_fine_client/this-is-fine-client/build/extern/googletest/googlemock/CMakeFiles/gmock_autogen.dir/AutogenInfo.json Debug
	cd /Users/eeleexx/This_is_fine_client/this-is-fine-client/build/extern/googletest/googlemock && /opt/homebrew/Cellar/cmake/3.28.3/bin/cmake -E touch /Users/eeleexx/This_is_fine_client/this-is-fine-client/build/extern/googletest/googlemock/gmock_autogen/timestamp

gmock_autogen: extern/googletest/googlemock/CMakeFiles/gmock_autogen
gmock_autogen: extern/googletest/googlemock/gmock_autogen/timestamp
gmock_autogen: extern/googletest/googlemock/CMakeFiles/gmock_autogen.dir/build.make
.PHONY : gmock_autogen

# Rule to build all files generated by this target.
extern/googletest/googlemock/CMakeFiles/gmock_autogen.dir/build: gmock_autogen
.PHONY : extern/googletest/googlemock/CMakeFiles/gmock_autogen.dir/build

extern/googletest/googlemock/CMakeFiles/gmock_autogen.dir/clean:
	cd /Users/eeleexx/This_is_fine_client/this-is-fine-client/build/extern/googletest/googlemock && $(CMAKE_COMMAND) -P CMakeFiles/gmock_autogen.dir/cmake_clean.cmake
.PHONY : extern/googletest/googlemock/CMakeFiles/gmock_autogen.dir/clean

extern/googletest/googlemock/CMakeFiles/gmock_autogen.dir/depend:
	cd /Users/eeleexx/This_is_fine_client/this-is-fine-client/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/eeleexx/This_is_fine_client/this-is-fine-client /Users/eeleexx/This_is_fine_client/this-is-fine-client/extern/googletest/googlemock /Users/eeleexx/This_is_fine_client/this-is-fine-client/build /Users/eeleexx/This_is_fine_client/this-is-fine-client/build/extern/googletest/googlemock /Users/eeleexx/This_is_fine_client/this-is-fine-client/build/extern/googletest/googlemock/CMakeFiles/gmock_autogen.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : extern/googletest/googlemock/CMakeFiles/gmock_autogen.dir/depend

