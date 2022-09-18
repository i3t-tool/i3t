#!/bin/bash

SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]:-$0}"; )" &> /dev/null && pwd 2> /dev/null; )"

find "$SCRIPT_DIR/../Source" -iname *.h -o -iname *.cpp | xargs clang-format -i
find "$SCRIPT_DIR/../Test"   -iname *.h -o -iname *.cpp | xargs clang-format -i
