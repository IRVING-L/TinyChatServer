cmake_minimum_required(VERSION 2.8)

project(demo)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(${PROJECT_SOURCE_DIR}/include)

#set(SRC_LIST src ./main.cpp)
aux_source_directory(src SRC_LIST)
add_executable(main main.cpp ${SRC_LIST})