cmake_minimum_required(VERSION 3.10)
project(TauCeti C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/eszFW/cmake/)

find_package(SDL2 REQUIRED)

add_subdirectory(external/eszFW)

include_directories(
    SYSTEM external/eszFW/src
    SYSTEM external/eszFW/external/cute_headers
    ${SDL2_INCLUDE_DIRS}
    )

set(TauCeti_SRCS
    src/main.c
    )

add_executable(
    ${PROJECT_NAME}
    ${TauCeti_SRCS})

target_link_libraries(
    ${PROJECT_NAME}
    eszFW)
