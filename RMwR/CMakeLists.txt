set(HEADER_LIST inc/RMwR.hpp)
set(SRC_LIST src/RMwR.cpp)
set(DEPENDENCIES_LIST commands)

add_library(RMwR STATIC ${SRC_LIST} ${HEADER_LIST})

target_include_directories(RMwR PUBLIC . inc src)

add_dependencies(RMwR ${DEPENDENCIES_LIST})
target_link_libraries(RMwR PUBLIC ${DEPENDENCIES_LIST})
