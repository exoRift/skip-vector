cmake_minimum_required(VERSION 3.0.0)
project(skip-vector VERSION 0.1.0 LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 11)

include(CTest)
enable_testing()
add_executable(benchmark test/benchmark.cpp)
add_test(test benchmark 100 100)

add_library(skip-vector INTERFACE lib/SkipVector.h)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
