set(HEADER_LIST inc/binary.hpp inc/message.hpp)
set(SRC_LIST src/binary.cpp src/message.cpp)

add_library(Binary STATIC ${SRC_LIST} ${HEADER_LIST})

target_include_directories(Binary PUBLIC . inc src)
