set(HEADER_LIST inc/IwRM.hpp)
set(SRC_LIST src/IwRM.cpp)
set(DEPENDENCIES_LIST commands)

add_library(IwRM STATIC ${SRC_LIST} ${HEADER_LIST})

target_include_directories(IwRM PUBLIC . inc src)

add_dependencies(IwRM ${DEPENDENCIES_LIST})
target_link_libraries(IwRM PUBLIC ${DEPENDENCIES_LIST})
