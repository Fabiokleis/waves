cmake_minimum_required(VERSION 3.24)
project(stbi)

set(STBI_LIB_NAME "stbi")
add_library(${STBI_LIB_NAME} INTERFACE)

target_include_directories(
    ${STBI_LIB_NAME}
    INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)
