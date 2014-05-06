cmake_minimum_required(VERSION 2.6)
set(BIN redis)
aux_source_directory(./ SRC)
add_library(${BIN} ${SRC})

