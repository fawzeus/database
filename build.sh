#!/usr/bin/env bash
# Linux build script — mirrors the root Makefile's module discovery.
# Headers live in <module>/include/<module>/, sources in <module>/src/.
set -e

mkdir -p build

src_dirs=$(find . -path ./build -prune -o -type d -name src -print | sort)
include_dirs=$(find . -path ./build -prune -o -type d -name include -print | sort)
sources=$(find $src_dirs -name '*.cpp' | sort)
includes=$(printf '%s\n' "$include_dirs" "$src_dirs" | sed '/^$/d; s/^/-I/')

g++ -std=c++17 -Wall -g $includes $sources -o build/database

echo "Build successful! -> build/database"
