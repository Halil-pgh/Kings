cmake_minimum_required(VERSION 3.26)
project(Trial)

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

add_executable(Trial ${SRC_FILES}
        src/Core/Random.cpp
        src/Core/Random.h)
target_include_directories(Trial PRIVATE src)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
target_compile_definitions(Trial PRIVATE SFML_STATIC)
target_link_libraries(Trial PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        sfml-network
)

target_precompile_headers(Trial PRIVATE src/pch.h)
