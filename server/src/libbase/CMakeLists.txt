cmake_minimum_required(VERSION 2.8)
set(BIN base)
aux_source_directory(./ SRC)
add_library(${BIN} ${SRC})
target_link_libraries(${BIN})

