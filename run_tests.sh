#!/bin/bash

# Exit on error
set -e

# Create the tests directory if it doesn't exist
mkdir -p tests

# Create the build directory if it doesn't exist
mkdir -p build

# Build the project and tests
cd build
cmake ..
cmake --build .

# Run the tests
./tests/raydial_tests

echo "Tests completed!" 