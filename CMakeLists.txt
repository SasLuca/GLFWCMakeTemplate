cmake_minimum_required(VERSION 3.17)
project(glfw-project C)

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
add_executable(glfw-project "src/main.c")
target_include_directories(glfw-project PUBLIC "src")
target_link_libraries(glfw-project PRIVATE "glad" "glfw")
