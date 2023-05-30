set(HEADER_LIST inc/logic.hpp)
set(SRC_LIST src/logic.cpp)
set(DEPENDENCIES_LIST commands)

add_library(logic STATIC ${SRC_LIST} ${HEADER_LIST})

target_include_directories(logic PUBLIC . inc src)

add_dependencies(logic ${DEPENDENCIES_LIST})
target_link_libraries(logic PUBLIC ${DEPENDENCIES_LIST})
