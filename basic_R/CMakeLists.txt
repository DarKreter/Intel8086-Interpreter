set(HEADER_LIST inc/basic_R.hpp)
set(SRC_LIST src/basic_R.cpp)
set(DEPENDENCIES_LIST commands)

add_library(basic_R STATIC ${SRC_LIST} ${HEADER_LIST})

target_include_directories(basic_R PUBLIC . inc src)

add_dependencies(basic_R ${DEPENDENCIES_LIST})
target_link_libraries(basic_R PUBLIC ${DEPENDENCIES_LIST})
