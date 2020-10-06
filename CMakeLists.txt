cmake_minimum_required(VERSION 3.17)
project(opengl_experiments C)

set(CMAKE_C_STANDARD 99)

# Setup GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(libs/glfw EXCLUDE_FROM_ALL)

# Setup GLAD
add_library(glad "libs/glad/glad.c")
target_include_directories(glad PUBLIC "libs" "libs/glad")

# Setup executable
add_executable(opengl_experiments "src/main.c")
target_include_directories(opengl_experiments PUBLIC "src")
target_link_libraries(opengl_experiments PRIVATE "glad" "glfw")
