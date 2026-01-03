#!/bin/bash

# Remove existing build directory
rm -rf build

# Configure CMake from the root directory
cmake -S . -B build

# Build the project
cmake --build build

# Run the project using the custom 'run' target
cmake --build build --target run
