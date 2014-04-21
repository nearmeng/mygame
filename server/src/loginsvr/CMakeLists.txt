cmake_minimum_required(VERSION 2.8)
set(BIN loginsvr)
aux_source_directory(./ SRC)
add_executable(${BIN} ${SRC})
target_link_libraries(${BIN} -L${SRC_ROOT}/lib libprotocol.a -L${SRC_ROOT}/lib libredis.a)
target_link_libraries(${BIN} -L${SRC_ROOT}/lib libbase.a)
target_link_libraries(${BIN} libevent.a libevent_core.a libglog.a libprotobuf.a)
target_link_libraries(${BIN} libhiredis.a -lrt -lpthread)


