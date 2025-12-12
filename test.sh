#!/usr/bin/env bash
INCLUDE="$PWD"

# Run example 01
echo "Running example 01..."
clang -std=c++17 -target x86_64-pc-windows-msvc -I"$INCLUDE" examples/01-digraph.cxx
./a
echo ""

# Run example 02
echo "Running example 02..."
clang -std=c++17 -fopenmp -target x86_64-pc-windows-msvc -I"$INCLUDE" examples/02-digraph.cxx
./a
echo ""

# Run example 03
echo "Running example 03..."
clang -std=c++17 -fopenmp -target x86_64-pc-windows-msvc -I"$INCLUDE" examples/03-loadmtx.cxx
./a examples/mycielskian5.mtx
echo ""

# Run example 04
echo "Running example 04..."
clang -std=c++17 -fopenmp -target x86_64-pc-windows-msvc -I"$INCLUDE" examples/04-batchupdate.cxx
./a examples/mycielskian5.mtx
echo ""

# Run example 05
echo "Running example 05..."
clang -std=c++17 -fopenmp -target x86_64-pc-windows-msvc -I"$INCLUDE" examples/05-transpose.cxx
./a
echo ""

# Run example 06
# echo "Running example 06..."
# clang -std=c++17 -fopenmp -target x86_64-pc-windows-msvc -I"$INCLUDE" examples/06-leiden.cxx
# ./a examples/mycielskian5.mtx
