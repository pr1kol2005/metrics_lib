cmake_minimum_required(VERSION 3.20)
project(metrics_lib)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Explicit: cmake -B build -D BUILD_TESTS=ON -D BUILD_TESTS=ON
option(BUILD_TESTS "Build tests" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(TESTS_DIR ${PROJECT_SOURCE_DIR}/tests)
set(EXTERNAL_DIR ${PROJECT_SOURCE_DIR}/external)
set(EXAMPLES_DIR ${PROJECT_SOURCE_DIR}/examples)

add_subdirectory(${SOURCE_DIR})

if(BUILD_EXAMPLES)
  add_subdirectory(${EXAMPLES_DIR})
endif()

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(${TESTS_DIR})
  add_subdirectory(${EXTERNAL_DIR})
endif()
