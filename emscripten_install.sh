#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

./emscripten.sh
cp web_src/* /var/www/html/stl/
cp build_emscripten/* /var/www/html/stl/

