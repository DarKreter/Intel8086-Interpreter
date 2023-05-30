set(HEADER_LIST inc/IwA.hpp)
set(SRC_LIST src/IwA.cpp)
set(DEPENDENCIES_LIST commands)

add_library(IwA STATIC ${SRC_LIST} ${HEADER_LIST})

target_include_directories(IwA PUBLIC . inc src)

add_dependencies(IwA ${DEPENDENCIES_LIST})
target_link_libraries(IwA PUBLIC ${DEPENDENCIES_LIST})
