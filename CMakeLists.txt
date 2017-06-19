cmake_minimum_required (VERSION 3.1.0)
project(ThreadPool)

add_subdirectory(Abstract)

set(SRC_LIST "src/RThreadPool.cpp")
include_directories(src)

add_library(ThreadPool ${SRC_LIST})

target_link_libraries(ThreadPool LINK_PUBLIC Abstract)
