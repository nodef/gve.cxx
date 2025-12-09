#!/usr/bin/env bash
clang -std=c++17 -fopenmp -target x86_64-pc-windows-msvc test/main.cxx
./a test/mycielskian5.mtx
