#!/bin/bash

set -eo pipefail

EXECUTABLE="clang-format"
if command -v "$EXECUTABLE"-16 &> /dev/null; then
  EXECUTABLE="clang-format-16"
fi

echo "Using $($EXECUTABLE --version)"

find "Source" -iname *.h -o -iname *.cpp | xargs "$EXECUTABLE" -dry-run -Werror
find "Test"   -iname *.h -o -iname *.cpp | xargs "$EXECUTABLE" -dry-run -Werror
