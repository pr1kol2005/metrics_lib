#!/bin/bash

echo "Launching clang-format check..."

find src tests -name '*.hpp' -o -name '*.cpp' | while read file; do
  clang-format -i "$file";
done

git diff --no-index --ignore-submodules --color > diff.txt

cat diff.txt
if [[ -s diff.txt ]]; then
  echo "❌ clang-format failed";
  exit 1;
fi

echo "✅ clang-format check passed"

exit 0
