#!/bin/bash

cmake -S . -B build \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

echo "Running CppCheck..."

output=$(cppcheck --enable=all \
    --inline-suppr \
    --file-filter='Source/*' \
    --template='{file}:{line}: {severity}: {message} [{id}]' \
    --suppress=preprocessorErrorDirective \
    --suppress=internalAstError \
    --suppress=missingIncludeSystem \
    --suppress=unknownMacro \
    --suppress=unusedFunction \
    --project=build/compile_commands.json 2>&1)
result=$?

relevant_output=$(echo "$output" | grep -e '^Source')

if [ $(echo "$relevant_output" | wc -l) -eq 0 ]; then
    echo "No issues found."
    exit 0
else
    echo "Issues found:"
    echo "$relevant_output"
    exit 1
fi
