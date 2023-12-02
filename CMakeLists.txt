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

set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/.cache/CPM")

CPMAddPackage(
  NAME Boost
  VERSION 1.83.0
  GITHUB_REPOSITORY "boostorg/boost"
  GIT_TAG "boost-1.83.0"
)


CPMAddPackage(
  NAME SCN
  VERSION 1.1.3
  GITHUB_REPOSITORY "eliaskosunen/scnlib"
  GIT_TAG "v1.1.3"
)

set(INPUT_DIR "\"${CMAKE_SOURCE_DIR}/inputs/\"")
add_compile_definitions(INPUT_DIR=${INPUT_DIR})

add_subdirectory(day01)
add_subdirectory(day02)