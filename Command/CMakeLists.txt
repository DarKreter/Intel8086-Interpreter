set(HEADER_LIST inc/commands.hpp)
set(SRC_LIST src/commands.cpp)
set(DEPENDENCIES_LIST Binary)

add_library(commands STATIC ${SRC_LIST} ${HEADER_LIST})

target_include_directories(commands PUBLIC . inc src)
add_dependencies(commands ${DEPENDENCIES_LIST})
target_link_libraries(commands PUBLIC ${DEPENDENCIES_LIST})
