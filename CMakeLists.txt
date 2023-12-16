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
  NAME SCN
  VERSION 1.1.3
  GITHUB_REPOSITORY "eliaskosunen/scnlib"
  GIT_TAG "v1.1.3"
)

CPMAddPackage(
  NAME Boost
  VERSION 1.84.0.beta1
  GITHUB_REPOSITORY "boostorg/boost"
  GIT_TAG "boost-1.84.0.beta1"
)

set(INPUT_DIR "\"${CMAKE_SOURCE_DIR}/inputs/\"")
add_compile_definitions(INPUT_DIR=${INPUT_DIR})

add_subdirectory(day01)
add_subdirectory(day02)
add_subdirectory(day03)
add_subdirectory(day04)
add_subdirectory(day05)
add_subdirectory(day06)
add_subdirectory(day07)
add_subdirectory(day08)
add_subdirectory(day09)
add_subdirectory(day10)
add_subdirectory(day11)
add_subdirectory(day12)
add_subdirectory(day13)
add_subdirectory(day14)
add_subdirectory(day15)
add_subdirectory(day16)