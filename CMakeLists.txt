cmake_minimum_required(VERSION 3.0)
project(rplane C CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(Bullet CONFIG REQUIRED)
find_package(raylib REQUIRED)

include_directories(/usr/local/include/bullet)

OPTION(BULLET_PROFILE "Enable Timing and Performance Profiling" OFF)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib BulletDynamics BulletCollision LinearMath)
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
