#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

# Define variables
BUILD_DIR="build"
EXECUTABLE="./ContactDirectory"

echo "--- Build Process ---"

# 1. Create build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
fi

# 2. Navigate to build directory
cd "$BUILD_DIR"

# 3. Configure the project with CMake
echo "Configuring..."
cmake ..

# 4. Compile the project
echo "Compiling..."
make

# 5. Check if the executable exists and run it
if [ -f "$EXECUTABLE" ]; then
    echo "--- Running Program ---"
    echo ""
    $EXECUTABLE
else
    echo "Error: Executable not found."
    exit 1
fi
