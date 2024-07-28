cmake_minimum_required(VERSION 3.8)
project(battle-squares)

set(CMAKE_PREFIX_PATH "./vendor/")

add_subdirectory("./vendor/glad/")

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

file(GLOB_RECURSE SRC "./src/*.cpp" "./vendor/stb_image/src/stb_image.cpp")
file(COPY "./vendor/sdl2/lib/x64/SDL2.dll" DESTINATION ".")
file(COPY "./vendor/sdl2_mixer/lib/x64/SDL2_mixer.dll" DESTINATION ".")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-std=c++20")

add_executable(battle-squares "./main.cpp" ${SRC})

target_link_libraries(battle-squares glad SDL2::SDL2main SDL2::SDL2 SDL2_mixer::SDL2_mixer)

target_include_directories(battle-squares PRIVATE
    "./include"
    "./vendor/glad/include/"
    "./vendor/stb_image/include/"
)
