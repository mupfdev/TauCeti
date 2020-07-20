cmake_minimum_required(VERSION 3.10)
project(TauCeti C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/eszFW/cmake/)

find_package(SDL2 REQUIRED)

add_subdirectory(external/eszFW)

include_directories(
    ${SDL2_INCLUDE_DIRS})

set(TauCeti_sources src/main.c)

option(USE_LIBTMX "Use libTMX instead of cute_tiled" OFF)

if(USE_LIBTMX)
  add_definitions(-DUSE_LIBTMX)
else()
  add_definitions(-DUSE_CUTE_TILED)
endif(USE_LIBTMX)

add_executable(
    ${PROJECT_NAME}
    ${TauCeti_sources})

target_link_libraries(
    ${PROJECT_NAME}
    eszFW)
