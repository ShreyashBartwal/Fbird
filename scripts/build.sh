#!/bin/bash

set e

cd "$(dirname "$0")/.."

mkdir -p build_local

clang++ src/main.cpp -o build_local/flappy_bird \
    -std=c++17 \
    -I/opt/homebrew/include \
    -L/opt/homebrew/lib \
    -lraylib \
    -framework CoreVideo \
    -framework IOKit \
    -framework Cocoa \
    -framework GLUT \
    -framework OpenGL

echo "Build successful! Run your game with: ./build_local/flappy_bird"