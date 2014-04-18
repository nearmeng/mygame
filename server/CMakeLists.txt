
cmake_minimum_required(VERSION 2.8)
set(SRC_ROOT /home/patrick/Documents/mygame/server)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SRC_ROOT}/bin)
set(CMAKE_CONFIGURATION_TYPES Debug)
execute_process(COMMAND protoc --proto_path=${SRC_ROOT}/proto ${SRC_ROOT}/proto/loginsvr.proto --cpp_out=${SRC_ROOT}/proto)
include_directories(${SRC_ROOT})
add_subdirectory(${SRC_ROOT}/proto)
add_subdirectory(${SRC_ROOT}/src/loginsvr)
