#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

TARGET="$1"
mkdir -p $TARGET

./emscripten.sh
cp web_src/* $TARGET
cp build_emscripten/* $TARGET

