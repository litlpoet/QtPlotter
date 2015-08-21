cmake_minimum_required(VERSION 3.0.2)
project(QtPlot)

## qt plot setting
if (WIN32)
  list(APPEND CMAKE_PREFIX_PATH "c:/Local/msys64/mingw64/")
  set(LIB_EIGEN3_DIR "c:/Local/include/eigen3/" CACHE PATH "eigen3 directory")
else()
  list(APPEND CMAKE_PREFIX_PATH "/opt/qt5/5.5/gcc_64/")
  set(LIB_EIGEN3_DIR "/usr/local/include/eigen3/" CACHE PATH "eigen3 directory")
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
  set(CMAKE_CXX_FLAGS "${warnings}" CACHE STRING
    "C++ flags used by the compiler during all build types." FORCE)
  set(CMAKE_C_FLAGS "${warnings}" CACHE STRING
    "C flags used by the compiler during all build types." FORCE)
endif()

include_directories(SYSTEM ${LIB_EIGEN3_DIR})
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/plotter/*.cpp)
file(GLOB RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/plotter/*.qrc)
qt5_wrap_cpp(MOC_SOURCES ${SOURCES})
qt5_add_resources(MOC_RESOURCES ${RESOURCES})
add_library(plotter ${SOURCES} ${RESOURCES} ${MOC_RESOURCES} ${MOC_RESOURCES})
qt5_use_modules(plotter Core Gui Widgets)
target_compile_features(plotter PRIVATE cxx_range_for)

add_executable(qtplot src/main.cpp)
target_link_libraries(qtplot plotter Qt5::Core Qt5::Gui Qt5::Widgets)
target_compile_features(qtplot PRIVATE cxx_range_for)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CONFIGURED_ONCE TRUE CACHE INTERNAL
  "A flag showing that CMake has configured at least once")
