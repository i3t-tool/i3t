#!/bin/bash

find "Source" -iname *.h -o -iname *.cpp | xargs clang-format -i
find "Test"   -iname *.h -o -iname *.cpp | xargs clang-format -i
