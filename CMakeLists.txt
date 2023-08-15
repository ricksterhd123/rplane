cmake_minimum_required(VERSION 3.0)
project(rplane C CXX)
set(CMAKE_CXX_STANDARD 17)

# Remove this if you build statically...
link_directories(/usr/local/lib)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PUBLIC raylib)
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
