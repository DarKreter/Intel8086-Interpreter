set(HEADER_LIST inc/commands.hpp)
set(SRC_LIST src/commands.cpp)

add_library(commands STATIC ${SRC_LIST} ${HEADER_LIST})

target_include_directories(commands PUBLIC . inc src)
