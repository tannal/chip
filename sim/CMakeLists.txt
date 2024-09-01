


# 创建可执行文件
add_executable(matrix_multiplier_sim matrix_multiplier_sim.cpp)

# 链接 Verilator 库
target_link_libraries(matrix_multiplier_sim PRIVATE MatrixMultiplier)

# 添加包含目录
target_include_directories(matrix_multiplier_sim PRIVATE 
    ${CMAKE_SOURCE_DIR}/obj_dir
    ${VERILATOR_INCLUDE_DIR}
)

add_executable(matrix_multiplier_test test_matrix_multiplier.cpp)

# 链接 Verilator 库
target_link_libraries(matrix_multiplier_test PRIVATE MatrixMultiplier)

target_include_directories(matrix_multiplier_test PRIVATE 
    ${CMAKE_SOURCE_DIR}/obj_dir
    ${VERILATOR_INCLUDE_DIR}
)