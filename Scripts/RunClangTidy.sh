#!/bin/bash -eu

cmake -S . \
  -B build \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

# run-clang-tidy-17 is a script that runs clang-tidy-17 in parallel.
find "Source" -iname *.cpp | xargs run-clang-tidy-17 -j 4 -extra-arg='-stdlib=libc++' -p build
