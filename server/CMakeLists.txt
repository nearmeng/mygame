
cmake_minimum_required(VERSION 2.8)
set(SRC_ROOT ./)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SRC_ROOT}/bin)
set(CMAKE_CONFIGURATION_TYPES Debug)
add_subdirectory(${SRC_ROOT}/src/loginsvr)
