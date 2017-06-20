cmake_minimum_required (VERSION 3.1.0)
project(ThreadPool)

add_subdirectory(Abstract)

set(SRC_LIST "src/RThreadPool.cpp")
include_directories(src)

add_library(ThreadPool ${SRC_LIST})
target_include_directories (ThreadPool PUBLIC src)
target_link_libraries(ThreadPool LINK_PUBLIC Abstract)

option(Test "Test" OFF)


if (Test)
    add_executable(test-suite test/test.cpp)
    target_link_libraries(test-suite PUBLIC ThreadPool)
endif()
