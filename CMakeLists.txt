cmake_minimum_required(VERSION 3.10) 
project(I2C_TOOLS)                    

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32") 

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories("${CMAKE_SOURCE_DIR}/ch341par/")

add_executable(i2cset ${CMAKE_SOURCE_DIR}/src/i2c_set.c)
add_executable(i2cdump ${CMAKE_SOURCE_DIR}/src/i2c_dump.c)
add_executable(i2cdetect ${CMAKE_SOURCE_DIR}/src/i2c_detect.c)

target_link_libraries(i2cset "${CMAKE_SOURCE_DIR}/ch341par/CH347DLL.DLL")
target_link_libraries(i2cdump "${CMAKE_SOURCE_DIR}/ch341par/CH347DLL.DLL")
target_link_libraries(i2cdetect "${CMAKE_SOURCE_DIR}/ch341par/CH347DLL.DLL")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_custom_command(TARGET i2cset POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/ch341par/CH347DLL.DLL"
        $<TARGET_FILE_DIR:i2cset>
    COMMENT "Copying CH347DLL.DLL to output directory"
)