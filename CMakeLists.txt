cmake_minimum_required(VERSION 3.20)
project(metrics_lib)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# cmake -B build -DBUILD_TESTS=ON
option(BUILD_TESTS "Build tests" OFF)

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(TESTS_DIR ${PROJECT_SOURCE_DIR}/tests)
set(EXTERNAL_DIR ${PROJECT_SOURCE_DIR}/external)

include_directories(${SOURCE_DIR})

add_subdirectory(${SOURCE_DIR})
add_subdirectory(${EXTERNAL_DIR})

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(${TESTS_DIR})
endif()
