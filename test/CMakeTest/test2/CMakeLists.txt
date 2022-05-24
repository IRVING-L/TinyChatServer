cmake_minimum_required(VERSION 2.8)

project(CMakeTest2)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

#aux_source_directory(add.cpp main.cpp SRC_LIST)

set(SRC_LIST ./main.cpp ./add.cpp)

add_executable(main ${SRC_LIST})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)