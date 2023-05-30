cmake_minimum_required(VERSION 3.10)

project(Disassebmly LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# In case of emergency add this debugging flag do compiler option: ||-ggdb3||
set(CPU_OPTIONS -Werror -Wall -Wextra -pedantic -ffunction-sections -fdata-sections -Ofast)
add_compile_options(${CPU_OPTIONS})

add_subdirectory(Command)
add_subdirectory(basic_R)
add_subdirectory(IwA)
add_subdirectory(IwRM)
add_subdirectory(jmp)
add_subdirectory(logic)
add_subdirectory(rep)
add_subdirectory(RMwR)
add_subdirectory(Singleton)

add_subdirectory(firmware)
