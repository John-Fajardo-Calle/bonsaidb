#!/bin/bash
set -e
BUILD_DIR="build_bench"
rm -rf "$BUILD_DIR"
mkdir "$BUILD_DIR"
cd "$BUILD_DIR"
cmake ..
make -j$(nproc)
cd ..
RESULT_DIR="benchmarks/resultados"
mkdir -p "$RESULT_DIR"
"$BUILD_DIR/benchmarks/stress_test" > "$RESULT_DIR/bench.log"