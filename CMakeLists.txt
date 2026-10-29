cmake_minimum_required(VERSION 3.16)

project(JRLib
    VERSION 1.0
    LANGUAGES C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(CWasp
    src/allocator.c
    src/string
)

target_include_directories(JRLib
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
