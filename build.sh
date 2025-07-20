#! /usr/bin/bash

cmake -S . -B ./build --preset mingw-32 -DMINGW=TRUE -DCMAKE_BUILD_TYPE=Release
cmake --build ./build
