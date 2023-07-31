cmake_minimum_required(VERSION 2.8...3.27)
project(waves)
set(CMAKE_CXX_STANDARD 17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(ONLY_LIBS ON CACHE BOOL "" FORCE)

file(GLOB SOURCE ./src/main.cpp ./src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})

include_directories(${OPENGL_INCLUDE_DIRS})

add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/glew)
add_subdirectory(dependencies/stbi)
add_subdirectory(dependencies/glm)

find_package(OpenGL REQUIRED)

target_include_directories(${PROJECT_NAME} 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC dependencies/glfw/include
    PUBLIC dependencies/uglew/include
    PUBLIC dependencies/stbi/include
    PUBLIC dependencies/glm/glm
)

target_link_directories(${PROJECT_NAME} PUBLIC dependencies/glfw/src dependencies/glew/src)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw libglew_static glm::glm) 
