cmake_minimum_required(VERSION 3.10)
project(TauCeti C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/eszFW/cmake/)

add_subdirectory(external/eszFW)

include_directories(SYSTEM external/eszFW/src
        SYSTEM ${SDL2_INCLUDE_DIR}
        SYSTEM external/eszFW/external/tmx/src
        )

FILE(GLOB TauCeti_SRCS src/*.c)
add_executable(TauCeti ${TauCeti_SRCS})
target_link_libraries(TauCeti eszFW)
target_compile_options(TauCeti PRIVATE -pedantic-errors -Wall -Werror -Wextra)

set_property(TARGET TauCeti PROPERTY INTERPROCEDURAL_OPTIMIZATION True)
