#! /usr/bin/bash

cmake -S . -B ./build --preset mingw-32 -DMINGW=TRUE
cmake --build ./build
