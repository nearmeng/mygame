cmake_minimum_required(VERSION 2.8)
set(BIN redis)
file(GLOB CUR_HEADERS "*.h")
aux_source_directory(./ SRC)
add_library(${BIN} ${SRC} ${CUR_HEADERS})

