set(HEADER_LIST inc/singleton.hpp)
set(SRC_LIST src/singleton.cpp)
set(DEPENDENCIES_LIST commands)

add_library(Singleton STATIC ${SRC_LIST} ${HEADER_LIST})

target_include_directories(Singleton PUBLIC . inc src)

add_dependencies(Singleton ${DEPENDENCIES_LIST})
target_link_libraries(Singleton PUBLIC ${DEPENDENCIES_LIST})
