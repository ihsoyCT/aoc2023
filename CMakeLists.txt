cmake_minimum_required(VERSION 3.21)

if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 23)
endif()


set(CMAKE_CXX_EXTENSIONS OFF)

project(adventofcode CXX)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(StandardProjectSettings)
include(CompilerWarnings)
include(Cache)
include(CPM)

set(INPUT_DIR "\"${CMAKE_SOURCE_DIR}/inputs/\"")

add_compile_definitions(INPUT_DIR=${INPUT_DIR})

add_subdirectory(day01)