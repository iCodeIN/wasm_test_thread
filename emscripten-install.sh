#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

./emscripten.sh
cp build-emscripten/* /var/www/html/stl/
