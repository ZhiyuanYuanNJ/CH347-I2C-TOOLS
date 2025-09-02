cmake_minimum_required(VERSION 3.10)  # 指定CMake最低版本要求[1,4](@ref)
project(I2C_TOOLS)                      # 定义项目名称[1,4](@ref)

# 设置编译为32位程序
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32") 

# 设置C标准（可选，根据你的代码需要）
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 添加头文件搜索路径
include_directories("${CMAKE_SOURCE_DIR}/ch341par/")

add_executable(i2cset ${CMAKE_SOURCE_DIR}/src/i2c_set.c)
add_executable(i2cdump ${CMAKE_SOURCE_DIR}/src/i2c_dump.c)
add_executable(i2cdetect ${CMAKE_SOURCE_DIR}/src/i2c_detect.c)

target_link_libraries(i2cset "${CMAKE_SOURCE_DIR}/ch341par/CH347DLL.DLL")
target_link_libraries(i2cdump "${CMAKE_SOURCE_DIR}/ch341par/CH347DLL.DLL")
target_link_libraries(i2cdetect "${CMAKE_SOURCE_DIR}/ch341par/CH347DLL.DLL")

# 设置可执行文件的输出目录（可选，根据你的需求调整）
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# 添加一个自定义目标，用于将DLL文件复制到输出目录（如果链接需要）
add_custom_command(TARGET i2cset POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/ch341par/CH347DLL.DLL"
        $<TARGET_FILE_DIR:i2cset>
    COMMENT "Copying CH347DLL.DLL to output directory"
)