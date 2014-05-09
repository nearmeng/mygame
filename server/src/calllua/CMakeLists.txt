cmake_minimum_required(VERSION 2.8)
set(BIN calllua)
file(GLOB CUR_HEADERS "*.h")
aux_source_directory(./ SRC)
add_executable(${BIN} ${SRC} ${CUR_HEADERS})
target_link_libraries(${BIN} -L${SRC_ROOT}/lib libtoluacpp.a)
target_link_libraries(${BIN} libtolua++.a liblua.a)


