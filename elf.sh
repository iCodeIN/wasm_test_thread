#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

mkdir -p build_elf
g++ --std=c++11 src/*.cpp -o build_elf/stl.elf
