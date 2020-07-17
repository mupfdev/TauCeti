cmake_minimum_required(VERSION 3.10)
project(TauCeti C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/eszFW/cmake/)

find_package(SDL2 REQUIRED)

add_subdirectory(external/eszFW)

include_directories(
    ${SDL2_INCLUDE_DIRS})

set(TauCeti_sources src/main.c)

add_executable(
    ${PROJECT_NAME}
    ${TauCeti_sources})

target_link_libraries(
    ${PROJECT_NAME}
    eszFW)
