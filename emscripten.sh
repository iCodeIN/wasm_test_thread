#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

mkdir -p build_emscripten
em++ -s USE_PTHREADS=1 --std=c++11 src/*.cpp -o build_emscripten/stl.js
