cmake_minimum_required(VERSION 2.8)
# 设置工程名字
project(CMakeTest1)
# 编译选项
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)
# 输出可执行文件名称和作用关系
add_executable(main main.cpp)