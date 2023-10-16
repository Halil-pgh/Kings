#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

SOURCE_DIR="$SCRIPT_DIR/.."

if [ "$1" = "debug" ]; then
    CMAKE_BUILD_TYPE=Debug
    BUILD_DIR="$SOURCE_DIR/cmake-build-debug"
else
    CMAKE_BUILD_TYPE=Release
    BUILD_DIR="$SOURCE_DIR/cmake-build-release"
fi

cmake -G Ninja -DCMAKE_BUILD_TYPE="$CMAKE_BUILD_TYPE" -DBUILD_SHARED_LIBS=OFF -S "$SOURCE_DIR" -B "$BUILD_DIR"
ninja -C "$BUILD_DIR"