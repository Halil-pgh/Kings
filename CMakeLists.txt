cmake_minimum_required(VERSION 3.26)
project(Kings)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE SRC_FILES
        src/*.cpp
        src/*.h
)

add_executable(Kings ${SRC_FILES})
target_include_directories(Kings PRIVATE src)

target_compile_definitions(Kings PUBLIC SFML_STATIC)
target_link_libraries(Kings PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        sfml-network
)

target_precompile_headers(Kings PRIVATE src/pch.h)
