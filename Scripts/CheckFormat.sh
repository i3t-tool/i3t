#!/bin/bash

SCRIPT_DIR="$( cd -- "$( dirname -- "${BASH_SOURCE[0]:-$0}"; )" &> /dev/null && pwd 2> /dev/null; )"

find "Source" -iname *.h -o -iname *.cpp | xargs clang-format -dry-run -Werror
find "Test"   -iname *.h -o -iname *.cpp | xargs clang-format -dry-run -Werror
