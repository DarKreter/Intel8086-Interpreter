set(DEPENDENCIES_LIST commands Singleton jmp logic basic_R rep IwA IwRM RMwR)

add_executable(firmware src/main.cpp src/function.cpp)

target_include_directories(firmware PUBLIC . src inc)

add_dependencies(firmware ${DEPENDENCIES_LIST})
target_link_libraries(firmware PUBLIC ${DEPENDENCIES_LIST})
