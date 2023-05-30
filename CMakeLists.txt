cmake_minimum_required(VERSION 3.10)

project(Disassebmly LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# In case of emergency add this debugging flag do compiler option: ||-ggdb3||
set(CPU_OPTIONS -Werror -Wall -Wextra -pedantic -ffunction-sections -fdata-sections -Ofast)
add_compile_options(${CPU_OPTIONS})

add_executable(firmware src/basic_R.cpp src/logic.cpp src/IwRM.cpp src/RMwR.cpp src/jmp.cpp src/main.cpp src/function.cpp src/commands.cpp)

target_include_directories(firmware PUBLIC . inc src)

# add_dependencies(firmware ${DEPENDENCIES_LIST})
# target_link_libraries(firmware PUBLIC ${DEPENDENCIES_LIST})
