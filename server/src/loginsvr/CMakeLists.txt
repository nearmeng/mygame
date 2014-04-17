cmake_minimum_required(VERSION 2.8)
set(BIN loginsvr)
aux_source_directory(./ SRC)
add_executable(${BIN} ${SRC})
