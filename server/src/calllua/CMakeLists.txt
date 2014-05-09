cmake_minimum_required(VERSION 2.8)
set(BIN loginsvr)
file(GLOB CUR_HEADERS "*.h")
aux_source_directory(./ SRC)
add_executable(${BIN} ${SRC} ${CUR_HEADERS})


