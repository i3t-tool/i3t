#!/bin/bash

set -eo pipefail

find "Source" -iname *.h -o -iname *.cpp | xargs clang-format -dry-run -Werror
find "Test"   -iname *.h -o -iname *.cpp | xargs clang-format -dry-run -Werror
