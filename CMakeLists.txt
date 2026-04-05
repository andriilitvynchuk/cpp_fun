cmake_minimum_required(VERSION 3.20)
project(start VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
include(GNUInstallDirs)

# OpenCV (required for main; install: brew install opencv)
find_package(OpenCV REQUIRED)

# ---- quill logging (install: brew install quill) ----
find_package(quill REQUIRED)

# ---- Python bindings (optional) ----
option(BUILD_PYTHON "Build Python bindings (start_lib)" OFF)
if(BUILD_PYTHON)
  find_package(Python 3.8 REQUIRED COMPONENTS Interpreter Development)
  include(FetchContent)
  FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11
    GIT_TAG        v2.11.1
  )
  FetchContent_MakeAvailable(pybind11)
  pybind11_add_module(start_lib_py src/bindings.cpp)
  set_target_properties(start_lib_py PROPERTIES OUTPUT_NAME _start_lib PREFIX "")
  target_link_libraries(start_lib_py PRIVATE start_lib)
  target_include_directories(start_lib_py PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()

# Library: sources under src/, public API under include/
add_library(start_lib src/sensor.cpp)
target_include_directories(start_lib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
target_compile_options(start_lib PRIVATE -Wall -Wextra -Wpedantic)

# Main executable
add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE start_lib)
if(TARGET OpenCV::opencv_core)
  # Only opencv_core to avoid imgcodecs (OpenEXR) until opencv is reinstalled.
  target_link_libraries(main PRIVATE OpenCV::opencv_core)
else()
  target_include_directories(main PRIVATE ${OpenCV_INCLUDE_DIRS})
  # Link core only to avoid OpenEXR; add opencv_imgproc etc. after brew reinstall opencv.
  target_link_libraries(main PRIVATE opencv_core)
endif()
target_link_libraries(main PRIVATE quill::quill)
target_compile_options(main PRIVATE -Wall -Wextra -Wpedantic)

# Tests (optional: cmake -B build -DBUILD_TESTS=OFF to skip)
option(BUILD_TESTS "Build test targets" ON)
if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

# ---- Install for reuse (other projects: find_package(start CONFIG)) ----
install(TARGETS start_lib
  EXPORT startTargets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILES_MATCHING PATTERN "*.h"
)

install(EXPORT startTargets
  NAMESPACE start::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/start
  FILE startTargets.cmake
)

set(start_VERSION ${PROJECT_VERSION})
configure_file(cmake/startConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/startConfig.cmake
  @ONLY
)
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/startConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)
install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/startConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/startConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/start
)
