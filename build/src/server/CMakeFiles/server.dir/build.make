# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/ubuntu/tiny-chat-server

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/tiny-chat-server/build

# Include any dependencies generated for this target.
include src/server/CMakeFiles/server.dir/depend.make

# Include the progress variables for this target.
include src/server/CMakeFiles/server.dir/progress.make

# Include the compile flags for this target's objects.
include src/server/CMakeFiles/server.dir/flags.make

src/server/CMakeFiles/server.dir/chatserver.cpp.o: src/server/CMakeFiles/server.dir/flags.make
src/server/CMakeFiles/server.dir/chatserver.cpp.o: ../src/server/chatserver.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/tiny-chat-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/server/CMakeFiles/server.dir/chatserver.cpp.o"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server.dir/chatserver.cpp.o -c /home/ubuntu/tiny-chat-server/src/server/chatserver.cpp

src/server/CMakeFiles/server.dir/chatserver.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/chatserver.cpp.i"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/tiny-chat-server/src/server/chatserver.cpp > CMakeFiles/server.dir/chatserver.cpp.i

src/server/CMakeFiles/server.dir/chatserver.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/chatserver.cpp.s"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/tiny-chat-server/src/server/chatserver.cpp -o CMakeFiles/server.dir/chatserver.cpp.s

src/server/CMakeFiles/server.dir/chatservice.cpp.o: src/server/CMakeFiles/server.dir/flags.make
src/server/CMakeFiles/server.dir/chatservice.cpp.o: ../src/server/chatservice.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/tiny-chat-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/server/CMakeFiles/server.dir/chatservice.cpp.o"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server.dir/chatservice.cpp.o -c /home/ubuntu/tiny-chat-server/src/server/chatservice.cpp

src/server/CMakeFiles/server.dir/chatservice.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/chatservice.cpp.i"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/tiny-chat-server/src/server/chatservice.cpp > CMakeFiles/server.dir/chatservice.cpp.i

src/server/CMakeFiles/server.dir/chatservice.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/chatservice.cpp.s"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/tiny-chat-server/src/server/chatservice.cpp -o CMakeFiles/server.dir/chatservice.cpp.s

src/server/CMakeFiles/server.dir/server_main.cpp.o: src/server/CMakeFiles/server.dir/flags.make
src/server/CMakeFiles/server.dir/server_main.cpp.o: ../src/server/server_main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/tiny-chat-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/server/CMakeFiles/server.dir/server_main.cpp.o"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server.dir/server_main.cpp.o -c /home/ubuntu/tiny-chat-server/src/server/server_main.cpp

src/server/CMakeFiles/server.dir/server_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/server_main.cpp.i"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/tiny-chat-server/src/server/server_main.cpp > CMakeFiles/server.dir/server_main.cpp.i

src/server/CMakeFiles/server.dir/server_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/server_main.cpp.s"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/tiny-chat-server/src/server/server_main.cpp -o CMakeFiles/server.dir/server_main.cpp.s

src/server/CMakeFiles/server.dir/db/db.cpp.o: src/server/CMakeFiles/server.dir/flags.make
src/server/CMakeFiles/server.dir/db/db.cpp.o: ../src/server/db/db.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/tiny-chat-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/server/CMakeFiles/server.dir/db/db.cpp.o"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server.dir/db/db.cpp.o -c /home/ubuntu/tiny-chat-server/src/server/db/db.cpp

src/server/CMakeFiles/server.dir/db/db.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/db/db.cpp.i"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/tiny-chat-server/src/server/db/db.cpp > CMakeFiles/server.dir/db/db.cpp.i

src/server/CMakeFiles/server.dir/db/db.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/db/db.cpp.s"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/tiny-chat-server/src/server/db/db.cpp -o CMakeFiles/server.dir/db/db.cpp.s

src/server/CMakeFiles/server.dir/model/friendmodel.cpp.o: src/server/CMakeFiles/server.dir/flags.make
src/server/CMakeFiles/server.dir/model/friendmodel.cpp.o: ../src/server/model/friendmodel.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/tiny-chat-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/server/CMakeFiles/server.dir/model/friendmodel.cpp.o"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server.dir/model/friendmodel.cpp.o -c /home/ubuntu/tiny-chat-server/src/server/model/friendmodel.cpp

src/server/CMakeFiles/server.dir/model/friendmodel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/model/friendmodel.cpp.i"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/tiny-chat-server/src/server/model/friendmodel.cpp > CMakeFiles/server.dir/model/friendmodel.cpp.i

src/server/CMakeFiles/server.dir/model/friendmodel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/model/friendmodel.cpp.s"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/tiny-chat-server/src/server/model/friendmodel.cpp -o CMakeFiles/server.dir/model/friendmodel.cpp.s

src/server/CMakeFiles/server.dir/model/groupmodel.cpp.o: src/server/CMakeFiles/server.dir/flags.make
src/server/CMakeFiles/server.dir/model/groupmodel.cpp.o: ../src/server/model/groupmodel.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/tiny-chat-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/server/CMakeFiles/server.dir/model/groupmodel.cpp.o"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server.dir/model/groupmodel.cpp.o -c /home/ubuntu/tiny-chat-server/src/server/model/groupmodel.cpp

src/server/CMakeFiles/server.dir/model/groupmodel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/model/groupmodel.cpp.i"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/tiny-chat-server/src/server/model/groupmodel.cpp > CMakeFiles/server.dir/model/groupmodel.cpp.i

src/server/CMakeFiles/server.dir/model/groupmodel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/model/groupmodel.cpp.s"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/tiny-chat-server/src/server/model/groupmodel.cpp -o CMakeFiles/server.dir/model/groupmodel.cpp.s

src/server/CMakeFiles/server.dir/model/offlinemsgmodel.cpp.o: src/server/CMakeFiles/server.dir/flags.make
src/server/CMakeFiles/server.dir/model/offlinemsgmodel.cpp.o: ../src/server/model/offlinemsgmodel.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/tiny-chat-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object src/server/CMakeFiles/server.dir/model/offlinemsgmodel.cpp.o"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server.dir/model/offlinemsgmodel.cpp.o -c /home/ubuntu/tiny-chat-server/src/server/model/offlinemsgmodel.cpp

src/server/CMakeFiles/server.dir/model/offlinemsgmodel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/model/offlinemsgmodel.cpp.i"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/tiny-chat-server/src/server/model/offlinemsgmodel.cpp > CMakeFiles/server.dir/model/offlinemsgmodel.cpp.i

src/server/CMakeFiles/server.dir/model/offlinemsgmodel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/model/offlinemsgmodel.cpp.s"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/tiny-chat-server/src/server/model/offlinemsgmodel.cpp -o CMakeFiles/server.dir/model/offlinemsgmodel.cpp.s

src/server/CMakeFiles/server.dir/model/usermodel.cpp.o: src/server/CMakeFiles/server.dir/flags.make
src/server/CMakeFiles/server.dir/model/usermodel.cpp.o: ../src/server/model/usermodel.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/tiny-chat-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object src/server/CMakeFiles/server.dir/model/usermodel.cpp.o"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server.dir/model/usermodel.cpp.o -c /home/ubuntu/tiny-chat-server/src/server/model/usermodel.cpp

src/server/CMakeFiles/server.dir/model/usermodel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/model/usermodel.cpp.i"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/tiny-chat-server/src/server/model/usermodel.cpp > CMakeFiles/server.dir/model/usermodel.cpp.i

src/server/CMakeFiles/server.dir/model/usermodel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/model/usermodel.cpp.s"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/tiny-chat-server/src/server/model/usermodel.cpp -o CMakeFiles/server.dir/model/usermodel.cpp.s

src/server/CMakeFiles/server.dir/redis/redis.cpp.o: src/server/CMakeFiles/server.dir/flags.make
src/server/CMakeFiles/server.dir/redis/redis.cpp.o: ../src/server/redis/redis.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/tiny-chat-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object src/server/CMakeFiles/server.dir/redis/redis.cpp.o"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/server.dir/redis/redis.cpp.o -c /home/ubuntu/tiny-chat-server/src/server/redis/redis.cpp

src/server/CMakeFiles/server.dir/redis/redis.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/server.dir/redis/redis.cpp.i"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/tiny-chat-server/src/server/redis/redis.cpp > CMakeFiles/server.dir/redis/redis.cpp.i

src/server/CMakeFiles/server.dir/redis/redis.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/server.dir/redis/redis.cpp.s"
	cd /home/ubuntu/tiny-chat-server/build/src/server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/tiny-chat-server/src/server/redis/redis.cpp -o CMakeFiles/server.dir/redis/redis.cpp.s

# Object files for target server
server_OBJECTS = \
"CMakeFiles/server.dir/chatserver.cpp.o" \
"CMakeFiles/server.dir/chatservice.cpp.o" \
"CMakeFiles/server.dir/server_main.cpp.o" \
"CMakeFiles/server.dir/db/db.cpp.o" \
"CMakeFiles/server.dir/model/friendmodel.cpp.o" \
"CMakeFiles/server.dir/model/groupmodel.cpp.o" \
"CMakeFiles/server.dir/model/offlinemsgmodel.cpp.o" \
"CMakeFiles/server.dir/model/usermodel.cpp.o" \
"CMakeFiles/server.dir/redis/redis.cpp.o"

# External object files for target server
server_EXTERNAL_OBJECTS =

../bin/server: src/server/CMakeFiles/server.dir/chatserver.cpp.o
../bin/server: src/server/CMakeFiles/server.dir/chatservice.cpp.o
../bin/server: src/server/CMakeFiles/server.dir/server_main.cpp.o
../bin/server: src/server/CMakeFiles/server.dir/db/db.cpp.o
../bin/server: src/server/CMakeFiles/server.dir/model/friendmodel.cpp.o
../bin/server: src/server/CMakeFiles/server.dir/model/groupmodel.cpp.o
../bin/server: src/server/CMakeFiles/server.dir/model/offlinemsgmodel.cpp.o
../bin/server: src/server/CMakeFiles/server.dir/model/usermodel.cpp.o
../bin/server: src/server/CMakeFiles/server.dir/redis/redis.cpp.o
../bin/server: src/server/CMakeFiles/server.dir/build.make
../bin/server: src/server/CMakeFiles/server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ubuntu/tiny-chat-server/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Linking CXX executable ../../../bin/server"
	cd /home/ubuntu/tiny-chat-server/build/src/server && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/server/CMakeFiles/server.dir/build: ../bin/server

.PHONY : src/server/CMakeFiles/server.dir/build

src/server/CMakeFiles/server.dir/clean:
	cd /home/ubuntu/tiny-chat-server/build/src/server && $(CMAKE_COMMAND) -P CMakeFiles/server.dir/cmake_clean.cmake
.PHONY : src/server/CMakeFiles/server.dir/clean

src/server/CMakeFiles/server.dir/depend:
	cd /home/ubuntu/tiny-chat-server/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/tiny-chat-server /home/ubuntu/tiny-chat-server/src/server /home/ubuntu/tiny-chat-server/build /home/ubuntu/tiny-chat-server/build/src/server /home/ubuntu/tiny-chat-server/build/src/server/CMakeFiles/server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/server/CMakeFiles/server.dir/depend
