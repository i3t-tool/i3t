#!/bin/bash

echo "Formatting code of these files:"
find "Source" -iname *.h -o -iname *.cpp

find "Source" -iname *.h -o -iname *.cpp | xargs clang-format -i
find "Test"   -iname *.h -o -iname *.cpp | xargs clang-format -i
