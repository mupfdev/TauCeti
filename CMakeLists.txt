cmake_minimum_required(VERSION 3.10)
project(TauCeti C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/eszFW/cmake/)

add_subdirectory(external/eszFW)

include_directories(SYSTEM external/eszFW/src
        SYSTEM ${SDL2_INCLUDE_DIR}
        )

FILE(GLOB tauCeti_SRCS src/*.c)
add_executable(tauCeti ${tauCeti_SRCS})
target_link_libraries(tauCeti eszFW)
