set(HEADER_LIST inc/jmp.hpp)
set(SRC_LIST src/jmp.cpp)
set(DEPENDENCIES_LIST commands)

add_library(jmp STATIC ${SRC_LIST} ${HEADER_LIST})

target_include_directories(jmp PUBLIC . inc src)

add_dependencies(jmp ${DEPENDENCIES_LIST})
target_link_libraries(jmp PUBLIC ${DEPENDENCIES_LIST})
