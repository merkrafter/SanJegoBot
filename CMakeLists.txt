# the oldest version this project was tested with; can be reduced after careful
# investigation
cmake_minimum_required(VERSION 3.16.0)

project(
  sanjego
  VERSION 0.0.1
  DESCRIPTION "Algorithms for playing the Game of San Jego"
  HOMEPAGE_URL "https://github.com/merkrafter/sanjego_cpp"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# Static linking is enabled to improve start-up speed
add_library(sanjego STATIC)

target_sources(sanjego PRIVATE include/libsanjego/gameobjects.hpp
                               src/gameobjects.cpp)

# Internal file can import the header files directly.
target_include_directories(
  sanjego
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/libsanjego>
          $<INSTALL_INTERFACE:include/libsanjego>)
# External projects that link to this library can import the header files with a
# libsanjego prefix.
target_include_directories(
  sanjego PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                 $<INSTALL_INTERFACE:include>)

find_package(Catch2)
if(NOT Catch2_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v2.13.1)
  FetchContent_MakeAvailable(Catch2)
endif()
enable_testing()
add_subdirectory(test)
