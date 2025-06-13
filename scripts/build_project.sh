#!/bin/bash

echo "Starting the build..."

if [ "$1" == "BUILD_TESTS=ON" ]; then
  BUILD_TESTS="ON"
else
  BUILD_TESTS="OFF"
fi

if [ "$2" == "BUILD_EXAMPLES=ON" ]; then
  BUILD_EXAMPLES="ON"
else
  BUILD_EXAMPLES="OFF"
fi

cmake -B build -D BUILD_TESTS=$BUILD_TESTS -D BUILD_EXAMPLES=$BUILD_EXAMPLES

cmake --build build

if [ $? -eq 0 ]; then
  echo "✅ Build successful!"
  exit 0
else
  echo "❌ Build failed!"
  exit 1
fi
