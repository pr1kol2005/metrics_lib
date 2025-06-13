#!/bin/bash

echo "Launching clang-tidy check..."

if [ ! -f build/compile_commands.json ]; then
  echo "❌ build/compile_commands.json is not found. Build project first."
  exit 1
fi

failed=0

echo "Check .hpp..."
find src tests -name '*.hpp' | while read file; do
  clang-tidy -p=build/compile_commands.json -quiet "$file" || {
    echo "❌ clang-tidy failed on $file"
    failed=1
  }
done

echo "Check .cpp..."
find src tests -name '*.cpp' | while read file; do
  clang-tidy -p=build/compile_commands.json -quiet "$file" || {
    echo "❌ clang-tidy failed on $file"
    failed=1
  }
done

if [ $failed -eq 1 ]; then
  exit 1
fi

echo "✅ clang-tidy check passed"

exit 0
