#!/bin/sh
CC=clang CXX=clang++ cmake -S . -B build -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug
