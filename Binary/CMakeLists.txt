set(HEADER_LIST inc/binary.hpp)
set(SRC_LIST src/binary.cpp)

add_library(Binary STATIC ${SRC_LIST} ${HEADER_LIST})

target_include_directories(Binary PUBLIC . inc src)
