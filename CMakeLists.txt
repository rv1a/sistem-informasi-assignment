cmake_minimum_required(VERSION 3.24)
project(sistem-informasi-splitted)

set(CMAKE_CXX_STANDARD 20)

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
    message("CCACHE FOUND YIPPEEE!")
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}" )
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}" )
endif()

add_executable(sistem-informasi-2 src/main.cpp src/lib/MenuMaker.cpp src/lib/Utils.cpp src/lib/Tugas.cpp src/lib/Datetime.cpp src/lib/MenuHandler.cpp)
target_include_directories(sistem-informasi-2 PRIVATE src/lib)