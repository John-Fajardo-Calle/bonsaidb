#!/bin/bash
set -e
BUILD_DIR="build_bench"
rm -rf "$BUILD_DIR"
mkdir "$BUILD_DIR"
cd "$BUILD_DIR"
cmake ..
make -j$(nproc)
cd ..
./stress_test > resultados/bench.log