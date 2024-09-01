cmake_minimum_required(VERSION 3.10)
project(MatrixMultiplier)

# 查找 Verilator
find_package(verilator HINTS $ENV{VERILATOR_ROOT})
if (NOT verilator_FOUND)
  message(FATAL_ERROR "Verilator was not found. Either install it, or set the VERILATOR_ROOT environment variable")
endif()

set(VERILATOR_INCLUDE_DIR ${VERILATOR_ROOT}/include)

add_library(MatrixMultiplier STATIC)

verilate(MatrixMultiplier
  SOURCES ${CMAKE_SOURCE_DIR}/generated/MatrixMultiplier.v  # 注意：这里应该是你的 Verilog 文件名
  TRACE
  TOP_MODULE MatrixMultiplier  # 确保这里是你的顶层模块名
  VERILATOR_ARGS -Wall
)

target_include_directories(MatrixMultiplier PUBLIC 
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/MatrixMultiplier.dir/MatrixMultiplier.dir
    ${VERILATOR_INCLUDE_DIR}
)

# 添加 sim 子目录
add_subdirectory(sim)