cmake_minimum_required(VERSION 3.1)
project(kitty LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

# Options
option(KITTY_BENCH "Build benchmarks" OFF)
option(KITTY_TEST "Build tests" OFF)

# clang specific compiler definitions
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES ".*Clang$")
  add_definitions(-fcolor-diagnostics)
endif()

set(CMAKE_CXX_FLAGS "-Wall")

include_directories(include)

if(KITTY_BENCH)
  add_subdirectory(bench)
endif()

if(KITTY_TEST)
  add_subdirectory(test)
endif()

