cmake_minimum_required(VERSION 3.0.2)
project(Plotter)

## library dependencies
set(LIBML_DIR "../libML" CACHE PATH "The path to the libML")
add_subdirectory(${LIBML_DIR} ${CMAKE_BINARY_DIR}/libML)
include_directories(SYSTEM ${LIBML_DIR})

## plotter setting
if (WIN32)
  list(APPEND CMAKE_PREFIX_PATH "c:/Local/msys64/mingw64/")
  set(EIGEN3_DIR "c:/Local/include/eigen3")
else()
  list(APPEND CMAKE_PREFIX_PATH "/opt/qt5/5.5/gcc_64/")
  set(EIGEN3_DIR "/usr/local/include/eigen3")
endif()

find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(warnings "-Wall -Wextra -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(warnings "/W4 /WX /EHsc")
endif()

if (NOT CONFIGURED_ONCE)
  set(CMAKE_CXX_FLAGS "${warnings}"
    CACHE STRING
    "C++ flags used by the compiler during all build types."
    FORCE)
  set(CMAKE_C_FLAGS "${warnings}"
    CACHE STRING
    "C flags used by the compiler during all build types."
    FORCE)
endif()

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${EIGEN3_DIR}
  )

file(GLOB SOURCES src/plotter/*.cpp)
add_executable(Plotter src/main.cpp ${SOURCES})
qt5_use_modules(Plotter Core Gui Widgets)
target_link_libraries(Plotter MLCore MLGaussian)
target_compile_features(Plotter PRIVATE cxx_range_for)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CONFIGURED_ONCE TRUE
  CACHE INTERNAL
  "A flag showing that CMake has configured at least once")