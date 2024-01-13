#!/bin/bash
#
echo Compiling project...
cmake --build build >/dev/null

echo Running project...
./build/pokedev
