cmake_minimum_required(VERSION 3.0)
project(rplane C)
set(CMAKE_C_STANDARD 99)

# This shouldn't be needed if you build statically...
link_directories(/usr/local/lib)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} PUBLIC raylib)
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
