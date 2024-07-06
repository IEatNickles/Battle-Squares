cmake_minimum_required(VERSION 3.8)
project(battle-squares)

add_subdirectory("./vendor/glad")
add_subdirectory("./vendor/glfw")

file(GLOB_RECURSE SRC "./src/*.cpp")

add_executable(battle-squares "./main.cpp" ${SRC})

target_link_libraries(battle-squares glad glfw)

target_include_directories(battle-squares PRIVATE
    "./include"
    "./vendor/glad/include/"
    "./vendor/glfw/include/"
)
