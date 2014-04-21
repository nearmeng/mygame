cmake_minimum_required(VERSION 2.8)
set(BIN loginsvr)
aux_source_directory(./ SRC)
add_executable(${BIN} ${SRC})
target_link_libraries(${BIN} -levent -levent_core -lglog -lprotobuf)
target_link_libraries(${BIN} -L${SRC_ROOT}/lib -lprotocol -L${SRC_ROOT}/lib -lredis)
target_link_libraries(${BIN} -L${SRC_ROOT}/lib -lbase)

