set(HEADER_LIST inc/rep.hpp)
set(SRC_LIST src/rep.cpp)
set(DEPENDENCIES_LIST commands)

add_library(rep STATIC ${SRC_LIST} ${HEADER_LIST})

target_include_directories(rep PUBLIC . inc src)

add_dependencies(rep ${DEPENDENCIES_LIST})
target_link_libraries(rep PUBLIC ${DEPENDENCIES_LIST})
