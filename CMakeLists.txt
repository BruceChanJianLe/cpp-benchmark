cmake_minimum_required (VERSION 3.10)
project(cpp-benchmark)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
  # "Debug", "Release", "MinSizeRel", "RelWithDebInfo"
  set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -Wdeprecated -fPIC -Wshadow")
  add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-Wnon-virtual-dtor>")
endif()

# Game Entity Library (example)
add_library(game_entity SHARED
  ./src/dod/gpame_entity.cpp
  ./src/oop/game_entity.cpp
)
target_include_directories(game_entity PUBLIC include)

include(FetchContent)

# Add GoogleTest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        main
)
FetchContent_MakeAvailable(googletest)

# Add GoogleBenchmark
include(FetchContent)
FetchContent_Declare(
  benchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        main
)
FetchContent_MakeAvailable(benchmark)

add_subdirectory(benchmark)
