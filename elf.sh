#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

mkdir -p build-elf
g++ --std=c++11 src/stl.cpp src/main.cpp -o build-elf/stl.elf
