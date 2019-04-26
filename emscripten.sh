#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

mkdir -p build-emscripten
emcc -s FORCE_FILESYSTEM=1 --std=c++11 src/stl.cpp src/main.cpp -o build-emscripten/stl.js
