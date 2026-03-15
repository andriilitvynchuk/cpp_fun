cmake_minimum_required(VERSION 3.20)
project(start CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Library: sources under src/, public API under include/
add_library(start_lib src/sensor.cpp)
target_include_directories(start_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(start_lib PRIVATE -Wall -Wextra -Wpedantic)

# Main executable
add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE start_lib)
target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic)

# Tests (optional: cmake -B build -DBUILD_TESTS=OFF to skip)
option(BUILD_TESTS "Build test targets" ON)
if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
